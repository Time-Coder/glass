import pickle
import os
import atexit
from pathlib import Path
import shutil

def save(filename, var):
	if not os.path.isdir(os.path.dirname(os.path.abspath(filename))):
		os.makedirs(os.path.dirname(os.path.abspath(filename)))
	file = open(filename, "wb")
	pickle.dump(var, file)
	file.close()

def load(filename):
	file = open(filename, "rb")
	var = pickle.load(file)
	file.close()
	return var

def purename(filename):
	basename = os.path.basename(filename)
	pos_point = basename.rfind(".")
	if pos_point == -1:
		return basename
	else:
		return basename[:pos_point]

def extname(filename):
	basename = os.path.basename(filename)
	pos_point = basename.rfind(".")
	if pos_point == -1:
		return ""
	else:
		return basename[pos_point+1:]

class Persis:
	var_dict = {}
	def __init__(self, root = "var"):
		self.deleted = False
		self.self_file_path = os.path.abspath(os.path.dirname(__file__))
		self.root = self.self_file_path + "/root/" + root
		if not os.path.isdir(self.root):
			os.makedirs(self.root)
		self.global_namespace_name = os.path.relpath(str(Path(self.root).resolve()), self.self_file_path + "/root").replace("\\", "::")

		self.namespace_list = []
		self.used_rel_namespace = set()
		self.used_abs_namespace = set()
		
		if len(Persis.var_dict) == 0:
			files = os.listdir(self.root)
			for file in files:
				self.__load(self.root + "/" + file)

		atexit.register(self.__on_del)

	def __del__(self):
		self.__on_del()

	def __getitem__(self, name):
		full_name = self.__full_name(name)
		if full_name in Persis.var_dict:
			return Persis.var_dict[full_name]
		else:
			raise NameError("name '" + name + "' is not defined in current scope")

	def __setitem__(self, name, value):
		Persis.var_dict[self.__full_name(name)] = value

	def __delitem__(self, name):
		full_name = self.__full_name(name)
		if full_name in Persis.var_dict:
			del Persis.var_dict[full_name]
			filename = self.__persis_filename(full_name)
			if os.path.isfile(filename):
				os.remove(filename)
		else:
			raise NameError("name '" + name + "' is not defined in current scope")

	def __iter__(self):
		return iter(self.keys())

	def __contains__(self, name):
		return (self.__full_name(name) in Persis.var_dict)

	def __len__(self):
		return len(self.keys())

	def __str__(self):
		return str(dict(self))

	def __repr__(self):
		return dict(self).__repr__()

	def __load(self, filename):
		if os.path.isfile(filename):
			if extname(filename) == "persis":
				Persis.var_dict[self.global_namespace_name + (self.namespace() + "::" + purename(filename)).replace("::::", "::")] = load(filename)
			return

		if os.path.isdir(filename):
			current_dir = os.getcwd()
			self.namespace(purename(filename))
			os.chdir(filename)
			files = os.listdir(".")
			if len(files) == 0:
				os.chdir(current_dir)
				shutil.rmtree(filename)
			else:
				for file in files:
					self.__load(file)
				os.chdir(current_dir)
			self.end_namespace()

	def __full_name(self, name):
		if len(name) >= 2 and name[:2] == "::":
			return self.global_namespace_name + name
		else:
			scopes = self.__searching_scopes()
			for scope in scopes:
				full_name = self.global_namespace_name + scope + "::" + name
				if full_name in Persis.var_dict:
					return full_name

			for space in self.used_abs_namespace:
				full_name = self.global_namespace_name + space + "::" + name
				if full_name in Persis.var_dict:
					return full_name

			for space in self.used_rel_namespace:
				for scope in scopes:
					full_name = self.global_namespace_name + scope + "::" + space + "::" + name
					if full_name in Persis.var_dict:
						return full_name

			return self.global_namespace_name + (self.namespace() + "::" + name).replace("::::", "::")

	def __searching_scopes(self):
		result = []
		length = len(self.namespace_list)
		for i in range(length):
			result.append("::" + "::".join(self.namespace_list[:length-i]))
		result.append("")
		return result

	def __persis_filename(self, full_name):
		return self.self_file_path + "/root/" + full_name.replace("::", "/") + ".persis"

	def __on_del(self):
		if not self.deleted:
			try:
				self.flush()
				self.deleted = True
			except:
				pass

	def namespace(self, scope = None):
		if scope == None:
			return "::" + "::".join(self.namespace_list)
		else:
			if "::" in scope:
				raise NameError("cannot contain '::' in namespace name")

			self.namespace_list.append(scope)

	def end_namespace(self):
		if len(self.namespace_list) != 0:
			self.namespace_list.pop()

	def using_namespace(self, scope):
		if len(scope) >= 2 and scope[:2] == "::":
			self.used_abs_namespace.add(scope)
		else:
			self.used_rel_namespace.add(scope)

	def end_using_namespace(self, scope):
		if len(scope) >= 2 and scope[:2] == "::":
			if scope in self.used_abs_namespace:
				self.used_abs_namespace.remove(scope)
		else:
			if scope in self.used_rel_namespace:
				self.used_rel_namespace.remove(scope)

	def pop(self, name):
		full_name = self.__full_name(name)
		if full_name in Persis.var_dict:
			value = Persis.var_dict.pop(full_name)
			filename = self.__persis_filename(full_name)
			if os.path.isfile(filename):
				os.remove(filename)
			return value
		else:
			raise NameError("name '" + name + "' is not defined in current scope")

	def clear(self):
		keys = self.keys()
		for key in keys:
			del self[key]

	def flush(self, name = None):
		if name != None:
			full_name = self.__full_name(name)
			if full_name not in Persis.var_dict:
				raise NameError("name '" + name + "' is not defined in current scope")

			save(self.__persis_filename(full_name), Persis.var_dict[full_name])
		else:
			for full_name in Persis.var_dict:
				if full_name.find(self.global_namespace_name + "::") == 0:
					save(self.__persis_filename(full_name), Persis.var_dict[full_name])

	def locals(self):
		result = {}
		current_namespace = self.global_namespace_name + (self.namespace() + "::").replace("::::", "::")
		for full_name in Persis.var_dict:
			if full_name.find(current_namespace) == 0:
				name = full_name[len(current_namespace):]
				if len(name) != 0 and name.find("::") == -1:
					result[name] = Persis.var_dict[full_name]
		return result

	def globals(self):
		result = {}
		for full_name in Persis.var_dict:
			if full_name.find(self.global_namespace_name + "::") == 0:
				name = full_name[len(self.global_namespace_name + "::"):]
				if len(name) != 0 and name.find("::") == -1:
					result[name] = Persis.var_dict[full_name]

		return result

	def keys(self):
		keys = []
		for full_name in Persis.var_dict:
			if full_name.find(self.global_namespace_name + "::") == 0:
				keys.append(full_name[len(self.global_namespace_name):])

		return keys

	def values(self):
		values = []
		for full_name in Persis.var_dict:
			if full_name.find(self.global_namespace_name + "::") == 0:
				values.append(Persis.var_dict[full_name])

		return values

	def items(self):
		items = []
		for key,value in Persis.var_dict.items():
			if key.find(self.global_namespace_name + "::") == 0:
				items.append((key,value))

		return items