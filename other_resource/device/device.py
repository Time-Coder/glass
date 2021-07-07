import os
import shutil
import json
import time as ti
import datetime
import BAC0
BAC0.log_level('critical', log_this=False)

from schedule import *
import bacpypes.object
from bacpypes.basetypes import DateTime
from bacpypes.primitivedata import Date, Time
import threading
from simple_test import *

def save(filename, var):
	with open(filename, 'w') as output:
		json.dump(var, output, indent=4)
 
def load(filename):
	with open(filename, 'r') as file:
		var = json.load(file)
	return var

def folder_modify_time(dirname):
	if not os.path.isdir(dirname):
		return 0

	max_mtime = 0
	files = os.listdir(dirname)
	for file in files:
		file = dirname + "/" + file
		mtime = 0
		if os.path.isfile(file):
			mtime = os.path.getmtime(file)
		elif os.path.isdir(file):
			mtime = folder_modify_time(file)

		if mtime > max_mtime:
			max_mtime = mtime

	return max_mtime

class Device:
	def __init__(self, device_name = None):
		self.connect(device_name)

	def connect(self, device_name = None):
		self.__repeat_write_dict = {}
		self.__repeat_write_thread = None
		self.__stop_all_write = False

		self.__in_recover_mode = False
		self.__original_dict = {}

		self.__ip_info = self.__ipconfig()
		self.__bacnet = None
		self.__bacnet_hot = None
		self.__bacnet_wifi = None

		self.__current_device = None
		self.__devices = []
		self.__devices_hot = []
		self.__devices_wifi = []
		self.__device_dict = {}
		self.__device_dict_hot = {}
		self.__device_dict_wifi = {}

		self.__current_map = ""
		self.__default_map_dir = os.path.abspath(os.path.dirname(__file__)).replace("\\", "/") + '/map'
		if not os.path.isdir(self.__default_map_dir):
			os.makedirs(self.__default_map_dir)

		connected1 = self.connect_hotspot(self.__computer_hotspot_ip())
		connected2 = self.connect_wifi(self.__wifi_ip())

		if connected1:
			self.__bacnet = self.__bacnet_hot
			self.__devices = self.__devices_hot
			self.__device_dict = self.__device_dict_hot
		elif connected2:
			self.__bacnet = self.__bacnet_wifi
			self.__devices = self.__devices_wifi
			self.__device_dict = self.__device_dict_wifi
		else:
			info("Error: No available device!")
			exit()

		self.__first_call_list = True
		self.__object_list = []

		self.__name2id = {}
		self.__id2name = {}

		if device_name != None:
			self.use_device(device_name)

	def __del__(self):
		self.__stop_all_write = True
		if not self.__repeat_write_thread is None and self.__repeat_write_thread.is_alive():
			self.__repeat_write_thread.join()

	def start_recover(self):
		if self.__in_recover_mode:
			return

		self.__in_recover_mode = True
		self.__original_dict = {}
		self.__original_local_datetime = None
		self.__original_system_time = None

	def end_recover(self):
		if not self.__in_recover_mode:
			return

		self.__in_recover_mode = False

		if self.__original_local_datetime:
			self.time_sync(self.__original_local_datetime + datetime.timedelta(seconds=ti.time() - self.__original_system_time))

		for object_id in self.__original_dict:
			for prop_name in self.__original_dict[object_id]:
				try:
					info("Recovering " + self.object_name(object_id) + " to", self.__original_dict[object_id][prop_name]["value"])
					if self.__original_dict[object_id][prop_name]["is_repeat"]:
						self.repeat_write(object_id, self.__original_dict[object_id][prop_name]["value"])
					else:
						self.stop_write(object_id)
						self.write(object_id, prop_name, self.__original_dict[object_id][prop_name]["value"])
						self.write(object_id, "outOfService", False)
				except:
					pass

		self.__original_dict = {}
		self.__original_local_datetime = None
		self.__original_system_time = None

	def disconnect(self):
		self.stop_write("all")
		
		try:
			self.__bacnet_hot.disconnect()
		except:
			pass

		try:
			self.__bacnet_wifi.disconnect()
		except:
			pass

	def connect_hotspot(self, ip):
		# ip = "192.168.56.1"
		# ip = "169.254.69.161"
		if not ip:
			return False

		try:
			self.__bacnet_hot = BAC0.lite(ip=ip)
			# self.__bacnet = BAC0.connect(ip=ip)
		except Exception as e:
			info("Error: device is connected by other software(Yabe, VTS, etc.)")
			exit()

		local_devices = []
		times = 0
		while not local_devices and times <= 3:
			self.__bacnet_hot.whois()
			ti.sleep(1)

			local_devices = list(self.__bacnet_hot.discoveredDevices.keys())
			if local_devices:
				self.__devices_hot.extend(local_devices)
				for device in local_devices:
					name = self.__bacnet_hot.read(device[0] + " device " + str(device[1]) + " objectName")
					self.__device_dict_hot[device[0]] = {"id": str(device[1]), "name": name}

			times += 1

		return bool(local_devices)

	def connect_wifi(self, ip):
		if not ip:
			return False

		try:
			self.__bacnet_wifi = BAC0.lite(ip=ip)
			# self.__bacnet = BAC0.connect(ip=ip)
		except Exception as e:
			info("Error: device is connected by other software(Yabe, VTS, etc.)")
			exit()

		local_devices = []
		times = 0
		while not local_devices and times <= 3:
			self.__bacnet_wifi.whois()
			ti.sleep(1)

			local_devices = list(self.__bacnet_wifi.discoveredDevices.keys())
			if local_devices:
				self.__devices_wifi.extend(local_devices)
				for device in local_devices:
					name = self.__bacnet_wifi.read(device[0] + " device " + str(device[1]) + " objectName")
					self.__device_dict_wifi[device[0]] = {"id": str(device[1]), "name": name}

			times += 1

		return bool(local_devices)

	def device_list(self):
		devices = []
		for device in self.__devices:
			devices.append(device[0])

		return devices

	def use_device(self, ip):
		if not self.__current_device is None:
			self.end_recover()
		self.stop_write("all")
		if ip in self.__device_dict_hot:
			self.__bacnet = self.__bacnet_hot
			self.__devices = self.__devices_hot
			self.__device_dict = self.__device_dict_hot
			last_device = self.__current_device
			self.__current_device = ip
			if ip != last_device:
				self.__first_call_list = True
				self.__object_list = self.object_list()
				self.load_map()
		elif ip in self.__device_dict_wifi:
			self.__bacnet = self.__bacnet_wifi
			self.__devices = self.__devices_wifi
			self.__device_dict = self.__device_dict_wifi
			last_device = self.__current_device
			self.__current_device = ip
			if ip != last_device:
				self.__first_call_list = True
				self.__object_list = self.object_list()
				self.load_map()
		else:
			found = False
			for key in self.__device_dict_hot:
				if ip == self.__device_dict_hot[key]["name"]:
					self.__bacnet = self.__bacnet_hot
					self.__devices = self.__devices_hot
					self.__device_dict = self.__device_dict_hot
					last_device = self.__current_device
					self.__current_device = key
					if key != last_device:
						self.__first_call_list = True
						self.__object_list = self.object_list()
						self.load_map()
					found = True
					break
			if not found:
				for key in self.__device_dict_wifi:
					if ip == self.__device_dict_wifi[key]["name"]:
						self.__bacnet = self.__bacnet_wifi
						self.__devices = self.__devices_wifi
						self.__device_dict = self.__device_dict_wifi
						last_device = self.__current_device
						self.__current_device = key
						if key != last_device:
							self.__first_call_list = True
							self.__object_list = self.object_list()
							self.load_map()
						found = True
						break
			if not found:
				raise ValueError(ip + " is not a valid device!")

	def create_map(self, dirname = None):
		bar = progressbar.ProgressBar("Creating object_name <-> object_id mapping ...")
		i = 1

		if not self.__object_list:
			self.__object_list = self.object_list()

		n = len(self.__object_list)

		self.__name2id = {}
		self.__id2name = {}

		for object_id in self.__object_list:
			object_name = self.read(object_id, "objectName")
			if isinstance(object_name, str):
				self.__name2id[object_name.lower()] = object_id
			if isinstance(object_id, str):
				self.__id2name[object_id] = object_name

			bar.update(i/n)
			i += 1

		bar.close()

		self.save_map(dirname)

	def map_list(self):
		return os.listdir(self.__default_map_dir)

	def current_map(self):
		return self.__current_map

	def delete_map(self, dirname):
		if os.path.isdir(self.__default_map_dir + "/" + dirname):
			shutil.rmtree(self.__default_map_dir + "/" + dirname)

	def save_map(self, dirname = None):
		if len(self.__name2id) == 0 or len(self.__id2name) == 0:
			return

		if dirname is None:
			dirname = ti.strftime("map_on_%Y%m%d%H%M%S")

		dirname = self.__default_map_dir + "/" + dirname
		if not os.path.isdir(dirname):
			os.makedirs(dirname)

		save(dirname + "/name2id.json", self.__name2id)
		save(dirname + "/id2name.json", self.__id2name)
		self.__current_map = dirname.replace(self.__default_map_dir + "/", "")

	def load_map(self, dirname = None):
		if dirname is None:
			folder_names = self.map_list()

			modify_time = 0
			for folder in folder_names:
				folder = self.__default_map_dir + "/" + folder
				current_modify_time = folder_modify_time(folder)
				if current_modify_time > modify_time:
					dirname = folder
					modify_time = current_modify_time

			if dirname is None:
				self.create_map()
				return

		if "/" not in dirname and "\\" not in dirname:
			dirname = self.__default_map_dir + "/" + dirname

		try:
			self.__name2id = load(dirname + "/name2id.json")
			self.__id2name = load(dirname + "/id2name.json")
			self.__current_map = dirname.replace(self.__default_map_dir + "/", "")
		except:
			self.create_map()

	def device_id(self, name = None):
		if self.__current_device is None:
			self.use_device(self.__devices[0][0])

		if name is None:
			return "device:" + self.__device_dict[self.__current_device]["id"]
		else:
			for ip in self.__device_dict:
				if self.__device_dict[ip]["name"] == name:
					return "device:" + self.__device_dict[ip]["id"]
			return None

	def device_ip(self, name = None):
		if self.__current_device is None:
			self.use_device(self.__devices[0][0])

		if name is None:
			return self.__current_device
		else:
			for ip in self.__device_dict:
				if self.__device_dict[ip]["name"] == name:
					return ip
			return None

	def device_name(self, name = None):
		if self.__current_device is None:
			self.use_device(self.__devices[0][0])

		ip = self.__current_device
		if name is None:
			return self.read(self.device_id(), "objectName")
		else:
			self.write(self.device_id(), "objectName", name)

	def object_name(self, object_id):
		if self.__current_device is None:
			self.use_device(self.__devices[0][0])

		if object_id not in self.__id2name:
			return str(object_id)

		return str(self.__id2name[object_id])

	def object_id(self, object_name):
		if self.__current_device is None:
			self.use_device(self.__devices[0][0])

		if object_name.lower() not in self.__name2id:
			raise KeyError(object_name + " is not in current device!")

		return str(self.__name2id[object_name.lower()])

	def object_list(self):
		if self.__current_device is None:
			self.use_device(self.__devices[0][0])

		if self.__first_call_list:
			try:
				objects = self.__bacnet.read(self.__current_device + " device " + self.__device_dict[self.__current_device]["id"] + " objectList")
			except:
				self.__bacnet.read(self.__current_device + " device " + self.__device_dict[self.__current_device]["id"] + " objectName")
				objects = self.__bacnet.read(self.__current_device + " device " + self.__device_dict[self.__current_device]["id"] + " objectList")
			for i in range(len(objects)):
				objects[i] = str(objects[i][0]) + ':' + str(objects[i][1])
			self.__first_call_list = False
			return objects
		else:
			return self.__object_list

	def local_date(self):
		board_date = self.read(self.device_id(), "localDate")
		return datetime.date(board_date[0]+1900, board_date[1], board_date[2])

	def local_time(self):
		board_time = self.read(self.device_id(), "localTime")
		return datetime.time(*board_time)

	def local_datetime(self):
		board_date = self.read(self.device_id(), "localDate")
		board_time = self.read(self.device_id(), "localTime")
		return datetime.datetime(board_date[0]+1900, board_date[1], board_date[2], *board_time)

	def time_sync(self, date = None, time = None):
		if self.__in_recover_mode and self.__original_local_datetime is None:
			self.__original_local_datetime = self.local_datetime()
			self.__original_system_time = ti.time()

		if date is None and time is None:
			self.__bacnet.time_sync()
			return

		if time is None:
			if isinstance(date, str):
				if " " in date:
					date_time = date.split(" ")
					date = Date(date_time[0])
					time = Time(date_time[1])
				elif "-" in date:
					date = Date(date)
					time = Time(self.local_time().strftime("%H:%M:%S"))
				elif ":" in date:
					time = Time(date)
					date = Date(self.local_date().strftime("%Y-%m-%d"))
				else:
					raise ValueError(date + " is not in date or time format.")
			elif isinstance(date, datetime.datetime):
				time = Time(date.strftime("%H:%M:%S"))
				date = Date(date.strftime("%Y-%m-%d"))
			elif isinstance(date, datetime.date):
				date = Date(date.strftime("%Y-%m-%d"))
				time = Time(self.local_time().strftime("%H:%M:%S"))
			elif isinstance(date, datetime.time):
				time = Time(date.strftime("%H:%M:%S"))
				date = Date(self.local_date().strftime("%Y-%m-%d"))
			elif isinstance(date, Date):
				time = Time(self.local_time().strftime("%H:%M:%S"))
			elif isinstance(date, Time):
				time = copy.deepcopy(date)
				date = Date(self.local_date().strftime("%Y-%m-%d"))
			elif isinstance(date, DateTime):
				self.__bacnet.time_sync(datetime=date)
				return
			else:
				raise ValueError("Type of param is not recogonized.")

			self.__bacnet.time_sync(datetime=DateTime(date=date.value, time=time.value))
			return

		if isinstance(date, str):
			date = Date(date)
		elif isinstance(date, tuple) or isinstance(date, list):
			date = Date(str(date[0]) + "-" + str(date[1]) + "-" + str(date[2]))
		elif isinstance(date, datetime.date):
			date = Date(date.strftime("%Y-%m-%d"))
		elif not isinstance(date, Date):
			raise ValueError("Type of date is not recogonized.")

		if isinstance(time, str):
			time = Time(time)
		elif isinstance(time, tuple) or isinstance(time, list):
			if len(time) <= 2:
				time = Time(str(time[0]) + ":" + str(time[1]))
			else:
				time = Time(str(time[0]) + ":" + str(time[1]) + ":" + str(time[2]))

		elif isinstance(time, datetime.time):
			time = Time(time.strftime("%H:%M:%S"))
		elif not isinstance(date, Date):
			raise ValueError("Type of time is not recogonized.")

		self.__bacnet.time_sync(datetime=DateTime(date=date.value, time=time.value))

	def __format_prop_name(self, prop_name):
		if "_" not in prop_name and " " not in prop_name and "\t" not in prop_name:
			if prop_name[0] >= 'A' and prop_name[0] <= 'Z':
				prop_name = prop_name[0].lower() + prop_name[1:]
			return prop_name

		prop_name = strip(prop_name.replace("_", " ").replace("\t", " "))
		if prop_name[0] >= 'A' and prop_name[0] <= 'Z':
			prop_name = prop_name[0].lower() + prop_name[1:]

		items = prop_name.split(" ")

		prop_name = ""
		for i in range(len(items)):
			if items[i] != "":
				if not prop_name:
					prop_name = items[i]
				else:
					prop_name += items[i].capitalize()

		return prop_name

	def raw_read(self, cmd):
		i = 0
		while cmd[i] != ' ':
			i += 1
		ip = cmd[0:i]

		value = None
		try:
			if ip in self.__device_dict_hot:
				value = self.__bacnet_hot.read(cmd)
			elif ip in self.__device_dict_wifi:
				value = self.__bacnet_wifi.read(cmd)

			if value == "active":
				value = True
			elif value == "inactive":
				value = False
		except:
			pass

		return value

	def raw_write(self, cmd):
		i = 0
		while cmd[i] != ' ':
			i += 1
		ip = cmd[0:i]

		try:
			if ip in self.__device_dict_hot:
				self.__bacnet_hot.write(cmd)
			elif ip in self.__device_dict_wifi:
				self.__bacnet_wifi.write(cmd)
		except:
			pass

	def read(self, object_id, prop_name = "presentValue"):
		if self.__current_device is None:
			self.use_device(self.__devices[0][0])

		if object_id not in self.__object_list:
			if object_id.lower() not in self.__name2id:
				raise KeyError(object_id + " is not in current device!")
			object_id = self.__name2id[object_id.lower()]

		items = object_id.split(":")
		value = None
		try:
			value = self.__bacnet.read(self.__current_device + " " + items[0] + " " + items[1] + " " + prop_name)
			if value == "active":
				value = True
			elif value == "inactive":
				value = False
		except:
			return None

		return value

	def write(self, object_id, prop_name = "", value = {}):
		if prop_name == "" and value == {}:
			self.multi_write(object_id)
			return

		if self.__current_device is None:
			self.use_device(self.__devices[0][0])

		if value == {}:
			value = prop_name
			prop_name = "presentValue"

		if object_id not in self.__object_list:
			if object_id.lower() not in self.__name2id:
				raise KeyError(object_id + " is not in current device!")
			object_id = self.__name2id[object_id.lower()]

		if "binary" in object_id:
			if value:
				value = "active"
			else:
				value = "inactive"

		if value is None:
			return

		items = object_id.split(":")
		primitive = (float, int, str, bool)

		if self.__in_recover_mode:
			if object_id not in self.__original_dict:
				self.__original_dict[object_id] = {}
			if prop_name not in self.__original_dict[object_id]:
				self.__original_dict[object_id][prop_name] = {}
				self.__original_dict[object_id][prop_name]["value"] = self.read(object_id, prop_name)
				self.__original_dict[object_id][prop_name]["is_repeat"] = (prop_name == "presentValue" and object_id in self.__repeat_write_dict)

		try:
			if not isinstance(value, primitive) or (isinstance(value, str) and ' ' in value):
				self.__bacnet.complex_write(self.__current_device + " " + items[0] + " " + items[1] + " " + prop_name + " - - 8", value)
			else:
				self.__bacnet.write(self.__current_device + " " + items[0] + " " + items[1] + " " + prop_name + " " + str(value) + " - 8")
		except BAC0.core.io.IOExceptions.NoResponseFromController as e:
			self.__bacnet.write(self.__current_device + " " + items[0] + " " + items[1] + " outOfService True - 8")
			if not isinstance(value, primitive) or (isinstance(value, str) and ' ' in value):
				self.__bacnet.complex_write(self.__current_device + " " + items[0] + " " + items[1] + " " + prop_name + " - - 8", value)
			else:
				self.__bacnet.write(self.__current_device + " " + items[0] + " " + items[1] + " " + prop_name + " " + str(value) + " - 8")

	def multi_write(self, write_dict):
		if self.__current_device is None:
			self.use_device(self.__devices[0][0])

		args = []
		for object_name in write_dict:
			if object_name not in self.__object_list:
				object_id = self.object_id(object_name)
			else:
				object_id = object_name

			items = object_id.split(":")
			if isinstance(write_dict[object_name], dict):
				for prop_name in write_dict[object_name]:
					args.append(items[0] + " " + items[1] + " " + prop_name + " " + str(write_dict[object_name][prop_name]))
			else:
				args.append(items[0] + " " + items[1] + " presentValue " + str(write_dict[object_name]) + " - 8")

		self.__bacnet.writeMultiple(addr = self.__current_device, args=args)

	def __repeat_write(self):
		while not self.__stop_all_write:
			for object_id in self.__repeat_write_dict:
				try:
					self.write(object_id, "presentValue", self.__repeat_write_dict[object_id])
				except:
					pass
			for i in range(3*60):
				if self.__stop_all_write:
					return
				ti.sleep(1)

	def repeat_write(self, object_id, value):
		if object_id not in self.__object_list:
			if object_id.lower() not in self.__name2id:
				raise KeyError(object_id + " is not in current device!")
			object_id = self.__name2id[object_id.lower()]

		self.__stop_all_write = False
		self.__repeat_write_dict[object_id] = value
		self.write(object_id, "presentValue", value)

		if self.__repeat_write_thread is None:
			self.__repeat_write_thread = threading.Thread(target=self.__repeat_write, daemon=True)
			self.__repeat_write_thread.start()

	def stop_write(self, object_id):
		if object_id not in self.__object_list:
			if object_id.lower() not in self.__name2id and object_id != "all":
				raise KeyError(object_id + " is not in current device!")
			if object_id != "all":
				object_id = self.__name2id[object_id.lower()]

		if object_id in self.__repeat_write_dict:
			self.__repeat_write_dict.pop(object_id)
		elif object_id == "all":
			self.__stop_all_write = True
			if not self.__repeat_write_thread is None and self.__repeat_write_thread.is_alive():
				self.__repeat_write_thread.join()
				self.__repeat_write_thread = None
			self.__repeat_write_dict.clear()

	def verify_write(self, object_id1, value1, object_id2 = None, value2 = None, timeout = 180, must = False, repeat = True):
		if object_id2 is None:
			object_id2 = object_id1
		if value2 is None:
			value2 = value1

		if "ni_" in self.object_name(object_id1).lower() and repeat:
			self.repeat_write(object_id1, value1)
		else:
			self.write(object_id1, "presentValue", value1)

		elapse_time = 0
		write_delta = 0
		printed = False
		start_time = ti.time()
		while self.read(object_id2, "presentValue") != value2:
			ti.sleep(0.5)
			elapse_time += 0.5
			write_delta += 0.5

			if not printed and elapse_time >= 2:
				info("Waiting", object_id2, "change to", value2, "...", end = " ")
				printed = True

			if write_delta >= 2:
				self.write(object_id1, "presentValue", value1)
				write_delta = 0

			if ti.time()-start_time > timeout:
				info("Timeout")
				info(object_id2 + " is " + str(self.read(object_id2, "presentValue")) + " now.")
				if must:
					raise Exception(object_id2 + " not change to " + str(value2) + " in " + str(timeout) + " seconds.")
				else:
					return
		if printed:
			info("Successful. Waited " + str(round(ti.time()-start_time, 2)) + " seconds.")
		info("Successfully write " + str(value1) + " to " + object_id1)

	def wait_until(self, object_id, value, timeout = 8*120, must = False):
		info("Waiting", object_id, "change to", value, "...", end=" ")
		
		if not isinstance(value, tuple) and not isinstance(value, list):
			value = [value]

		start_time = ti.time()
		while ti.time()-start_time <= timeout:
			if self.read(object_id, "presentValue") in value:
				time_delta = ti.time()-start_time
				info("Successful. Waited " + str(round(time_delta, 2)) + " seconds.")
				return time_delta
			else:
				ti.sleep(0.5)
		info("Timeout. Waited " + str(timeout) + " seconds.")
		info(object_id + " is " + str(self.read(object_id, "presentValue")) + " now.")
		if must:
			raise ValueError(object_id + " not change to " + str(value) + " in " + str(timeout) + " seconds.")
		return timeout

	def wait_until_not(self, object_id, value, timeout = 8*120, must = False):
		info("Waiting", object_id, "change away from", value, "...", end=" ")
		
		if not isinstance(value, tuple) and not isinstance(value, list):
			value = [value]

		start_time = ti.time()
		while ti.time()-start_time <= timeout:
			if self.read(object_id, "presentValue") not in value:
				time_delta = ti.time()-start_time
				info("Successful. Waited " + str(round(time_delta, 2)) + " seconds.")
				return time_delta
			else:
				ti.sleep(0.5)
		info("Timeout. Waited " + str(timeout) + " seconds.")
		info(object_id + " is " + str(self.read(object_id, "presentValue")) + " now.")
		if must:
			raise ValueError(object_id + " not change away from " + str(value) + " in " + str(timeout) + " seconds.")
		return timeout

	def __getitem__(self, key):
		if isinstance(key, tuple) and len(key) == 2:
			return self.read(key[0], key[1])
		else:
			return self.read(key, "presentValue")

	def __setitem__(self, key, value):
		if isinstance(key, tuple) and len(key) == 2:
			self.write(key[0], key[1], value)
		else:
			self.write(key, "presentValue", value)

	def prop_list(self, object_id):
		if self.__current_device is None:
			self.use_device(self.__devices[0][0])

		if object_id not in self.__object_list:
			if object_id.lower() not in self.__name2id:
				raise KeyError(object_id + " is not in current device!")
			object_id = self.__name2id[object_id.lower()]
		
		props = []	
		items = object_id.split(":")
		object_type = items[0]
		object_type = object_type[0].upper() + object_type[1:]
		for prop in eval("bacpypes.object." + object_type + "Object.properties"):
			if not prop.optional:
				props.append(prop.identifier)
			else:
				try:
					self.__bacnet.read(self.__current_device + " " + items[0] + " " + items[1] + " " + prop.identifier)
					props.append(prop.identifier)
				except:
					pass
	
		return props

	def read_weekly_schedule(self):
		return WeeklySchedule(self.read("EnumSchedule", "weeklySchedule"))

	def write_weekly_schedule(self, schedule):
		if isinstance(schedule, dict):
			schedule = WeeklySchedule(schedule)

		self.write("EnumSchedule", "weeklySchedule", schedule.to_bacpypes())
		
	def read_holiday_schedule(self):
		return HolidaySchedule(self.read("EnumSchedule", "exceptionSchedule"))

	def write_holiday_schedule(self, schedule):
		if isinstance(schedule, list):
			schedule = HolidaySchedule(schedule)

		year = str(self.local_date())[:4]
		schedule.setyear(year)
		self.write("EnumSchedule", "exceptionSchedule", schedule.to_bacpypes())

	def __ipconfig(self):
		os.system("ipconfig>ipconfig.txt")
		file = open("ipconfig.txt")
		content = file.read()
		file.close()
		os.remove("ipconfig.txt")

		result = {}

		lines = content.split("\n")
		current_key = ""
		current_sub_key = ""
		for line in lines:
			if not line or line == "Windows IP Configuration":
				continue

			if line[0] != ' ' and line[-1] == ':':
				current_key = line[:-1]
				result[current_key] = {}
			elif ' :' in line:
				pair = line.split(" :")
				current_sub_key = pair[0].replace(".", "").strip()
				sub_value = pair[1].strip()
				if current_sub_key in result[current_key]:
					if isinstance(result[current_key][current_sub_key], list):
						result[current_key][current_sub_key].append(sub_value)
					else:
						first_value = result[current_key][current_sub_key]
						result[current_key][current_sub_key] = [first_value, sub_value]
				else:
					result[current_key][current_sub_key] = sub_value

			else:
				result[current_key][current_sub_key] = line.strip()

		return result

	def __mask_to_len(self, mask):
		items = mask.split(".")
		n = 0
		for item in items:
			item = bin(int(item))[2:]
			n += item.count('1')

		return str(n)

	def __computer_hotspot_ip(self):
		ip = ""
		mask = ""

		for key in self.__ip_info:
			if "Wireless LAN adapter Local Area Connection*" in key:
				if "IPv4 Address" in self.__ip_info[key]:
					if isinstance(self.__ip_info[key]["IPv4 Address"], list):
						ip = self.__ip_info[key]["IPv4 Address"][0]
						mask = self.__ip_info[key]["Subnet Mask"][0]
					else:
						ip = self.__ip_info[key]["IPv4 Address"]
						mask = self.__ip_info[key]["Subnet Mask"]
					break

		if ip:
			return ip + '/' + str(self.__mask_to_len(mask))
		else:
			return ""

	def __wifi_ip(self):
		ip = ""
		mask = ""

		if "Wireless LAN adapter Wi-Fi" in self.__ip_info:
			if "IPv4 Address" in self.__ip_info["Wireless LAN adapter Wi-Fi"]:
				if isinstance(self.__ip_info["Wireless LAN adapter Wi-Fi"]["IPv4 Address"], list):
					ip = self.__ip_info["Wireless LAN adapter Wi-Fi"]["IPv4 Address"][0]
					mask = self.__ip_info["Wireless LAN adapter Wi-Fi"]["Subnet Mask"][0]
				else:
					ip = self.__ip_info["Wireless LAN adapter Wi-Fi"]["IPv4 Address"]
					mask = self.__ip_info["Wireless LAN adapter Wi-Fi"]["Subnet Mask"]

		if ip:
			return ip + '/' + str(self.__mask_to_len(mask))
		else:
			return ""

if __name__ == "__main__":
	device = Device()