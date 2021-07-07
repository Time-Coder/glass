from datetime import date, timedelta
from bacpypes.basetypes import DailySchedule, TimeValue, CalendarEntry, DateRange, SpecialEvent, SpecialEventPeriod
from bacpypes.primitivedata import Integer, Null
from bacpypes.constructeddata import ArrayOf
import copy

month_dict = \
{
	1: "Jan.",
	2: "Feb.",
	3: "Mar.",
	4: "Apr.",
	5: "May ",
	6: "Jun.",
	7: "Jul.",
	8: "Aug.",
	9: "Sep.",
	10: "Oct.",
	11: "Nov.",
	12: "Dec.",
	"January": 1,
	"February": 2,
	"March": 3,
	"April": 4,
	"May": 5,
	"June": 6,
	"July": 7,
	"August": 8,
	"September": 9,
	"October": 10,
	"November": 11,
	"December": 12,
}

weekday_dict = \
{
	1: "Monday",
	2: "Tuesday",
	3: "Wednesday",
	4: "Thursday",
	5: "Friday",
	6: "Saturday",
	7: "Sunday",
	"Monday": 1,
	"Tuesday": 2,
	"Wednesday": 3,
	"Thursday": 4,
	"Friday": 5,
	"Saturday": 6,
	"Sunday": 7,
}

order_dict = \
{
	1: "First",
	2: "Second",
	3: "Third",
	4: "Fourth",
	5: "Fifth",

	"First": 1,
	"Second": 2,
	"Third": 3,
	"Fourth": 4,
	"Fifth": 5
}

class Event:
	def __init__(self, start_time = (0, 0, 0, 0), end_time = (0, 0, 0, 0), occupancy = 1):
		self.start_time = (0, 0, 0, 0)
		self.end_time = (0, 0, 0, 0)
		self.occupancy = 1

		if isinstance(start_time, str):
			items = start_time.split(":")
			self.start_time = (int(items[0]), int(items[1]), 0, 0)
		elif (isinstance(start_time, tuple) or isinstance(start_time, list)) and len(start_time) == 4:
			self.start_time = start_time

		if isinstance(end_time, str):
			items = end_time.split(":")
			self.end_time = (int(items[0]), int(items[1]), 0, 0)
		elif (isinstance(end_time, tuple) or isinstance(end_time, list)) and len(end_time) == 4:
			self.end_time = end_time

		if isinstance(occupancy, str):
			occupancy = self.format(occupancy)
			if occupancy == "unoccupied":
				self.occupancy = 1
			elif occupancy == "occupied":
				self.occupancy = 0
			elif occupancy == "standby":
				self.occupancy = 3
		elif occupancy in [1, 0, 3]:
			self.occupancy = occupancy

	def __eq__(self, event):
		return (str(self) == str(event))

	def format(self, key):
		return key.replace(" ", "_").lower()

	def num2str(self, num):
		result = str(num)
		if num < 10:
			result = '0' + result

		return result

	def __getitem__(self, key):
		key = self.format(key)
		if key == "start_time":
			return self.num2str(self.start_time[0]) + ":" + self.num2str(self.start_time[1])

		elif key == "end_time":
			return self.num2str(self.end_time[0]) + ":" + self.num2str(self.end_time[1])

		elif key == "occupancy":
			if self.occupancy == 1:
				return "Unoccupied"
			elif self.occupancy == 0:
				return "Occupied"
			elif self.occupancy == 3:
				return "Standby"

		else:
			raise KeyError(key)

	def __setitem__(self, key, value):
		key = self.format(key)
		if key == "start_time":
			if isinstance(value, str):
				hour_minu = value.split(":")
				hour = hour_minu[0]
				minu = hour_minu[1]
				self.start_time = (int(hour), int(minu), 0, 0)
			else:
				self.start_time = (value[0], value[1], 0, 0)

		elif key == "end_time":
			if isinstance(value, str):
				hour_minu = value.split(":")
				hour = hour_minu[0]
				minu = hour_minu[1]
				self.end_time = (int(hour), int(minu), 0, 0)
			else:
				self.end_time = (value[0], value[1], 0, 0)

		elif key == "occupancy":
			value = self.format(value)
			if value == "unoccupied":
				self.occupancy = 1
			elif value == "occupied":
				self.occupancy = 0
			elif value == "standby":
				self.occupancy = 3

		else:
			raise KeyError(key)

	def __repr__(self):
		return self["occupancy"] + ": " + self.num2str(self.start_time[0]) + ":" + self.num2str(self.start_time[1]) + " to " + self.num2str(self.end_time[0]) + ":" + self.num2str(self.end_time[1])

	def to_dict(self):
		result = \
		{
			"start_time": self["start_time"],
			"end_time": self["end_time"],
			"occupancy": self["occupancy"]
		}

		return result

	def to_cloud_format(self):
		result = \
		{
            "startTime": self["start_time"],
            "startValue": str(self.occupancy),
            "endTime": self["end_time"],
            "endValue": "1"
        }

	def from_dict(self, dict_contents):
		self["start_time"] = dict_contents["start_time"]
		self["end_time"] = dict_contents["end_time"]
		self["occupancy"] = dict_contents["occupancy"]

class Holiday:
	def __init__(self, holiday = None, year = 255):
		self.start_date = (255, 0, 0, 255)
		self.duration = 1
		self.events = []
		
		self.year = year
		self.for_all_years = True
		if self.year != 255:
			self.for_all_years = False
			self.year -= 1900

		if isinstance(holiday, dict):
			self.from_dict(holiday)
		elif holiday != None:
			self.from_bacpypes(holiday)

	def __eq__(self, holiday):
		return (str(self) == str(holiday))

	def __repr__(self):
		result = self.date2str(self.start_date) + "    " + self.duration2str(self.duration) + "\n"
		if not self.for_all_years:
			result = str(self.year) + " " + result

		if len(self.events) == 0:
			result += "    Unoccupied: 00:00 to 00:00"
		else:
			for i in range(len(self.events)):
				if i != len(self.events)-1:
					result += "    " + str(self.events[i]) + "\n"
				else:
					result += "    " + str(self.events[i])

		return result

	def to_string(self):
		result = self.date2str(self.start_date) + "    " + self.duration2str(self.duration) + "\n"
		if not self.for_all_years:
			result = str(self.year) + " " + result

		if len(self.events) == 0:
			result += "    Unoccupied: 00:00 to 00:00"
		else:
			for i in range(len(self.events)):
				if i != len(self.events)-1:
					result += "    " + str(self.events[i]) + "\n"
				else:
					result += "    " + str(self.events[i])

		return result

	def __getitem__(self, key):
		if isinstance(key, str):
			key = key.lower().replace(" ", "_")

		if key == "start_date":
			# return self.date2str(self.start_date)
			if len(self.start_date) == 4:
				return self.start_date[1:3]
			else:
				return self.start_date
		elif key == "duration":
			return self.duration
		elif isinstance(key, int):
			return self.events[key]

	def __setitem__(self, key, value):
		if isinstance(key, str):
			key = key.lower().replace(" ", "_")
			
		if key == "start_date":
			if isinstance(value, tuple):
				if len(value) == 2:
					self.start_date = (self.year, value[0], value[1], 255)
				elif len(value) == 3:
					self.start_date = value
		elif key == "duration":
			self.duration = int(value)
		elif isinstance(key, int):
			self.events[key] = value

	def __len__(self):
		return len(self.events)

	def add_event(self, event):
		self.events.append(copy.deepcopy(event))

	def delete_event(self, i):
		self.events.pop(i)

	def clear(self):
		self.events = []

	def date_minus(self, date1, date2):
		dateEnd = list(date1[1:3])
		dateStart = list(date2[1:3])
		if dateEnd > dateStart:
			dateEnd.insert(0, int(self.year))
		else:
			dateEnd.insert(0, int(self.year)+1)

		dateStart.insert(0, int(self.year))

		delta = date(*(dateEnd)) - date(*(dateStart))
		return delta.days

	def date_plus(self, date1, duration, year):
		date2 = date(int(year), date1[1], date1[2]) + timedelta(days = duration-1)
		return (255, date2.month, date2.day, 255)

	def day2str(self, day):
		day_str = str(day)
		post = "th"
		if day_str[-1] == "1":
			post = "st"
		elif day_str[-1] == "2":
			post = "nd"
		elif day_str[-1] == "3":
			post = "rd"
		if day_str == "11" or day_str == "12" or day_str == "13":
			post = "th"
		return day_str + post

	def duration2str(self, days):
		days_str = str(days)
		if days <= 1:
			return days_str + " day"
		else:
			return days_str + " days"

	def date2str(self, date_tupel):
		if len(date_tupel) == 4:
			return month_dict[date_tupel[1]] + " " + self.day2str(date_tupel[2])
		else:
			return month_dict[date_tupel[0]] + " " + self.day2str(date_tupel[1]) + " " + weekday_dict[date_tupel[2]]

	def from_bacpypes(self, holiday):
		holiday_dict = holiday.dict_contents()
	

		if "dateRange" in holiday_dict["period"]["calendarEntry"]:
			self.start_date = holiday_dict["period"]["calendarEntry"]["dateRange"]["startDate"]
			self.duration = self.date_minus(holiday_dict["period"]["calendarEntry"]["dateRange"]["endDate"], holiday_dict["period"]["calendarEntry"]["dateRange"]["startDate"]) + 1
			self.year = holiday_dict["period"]["calendarEntry"]["dateRange"]["startDate"][0]
		
		elif "date" in holiday_dict["period"]["calendarEntry"]:
			self.start_date = holiday_dict["period"]["calendarEntry"]["date"]
			self.duration = 1
			self.year = holiday_dict["period"]["calendarEntry"]["date"][0]

		elif "weekNDay" in holiday_dict["period"]["calendarEntry"]:
			self.start_date = holiday_dict["period"]["calendarEntry"]["weekNDay"]
			self.duration = 1
			self.year = 255

		for i in range(0, len(holiday_dict["listOfTimeValues"])):
			if holiday_dict["listOfTimeValues"][i]["value"].value != 1:
				if (i == len(holiday_dict["listOfTimeValues"])-1):
					self.add_event(Event(start_time=holiday_dict["listOfTimeValues"][i]["time"], end_time=(0, 0, 0, 0), occupancy=holiday_dict["listOfTimeValues"][i]["value"].value))
				else:
					self.add_event(Event(start_time=holiday_dict["listOfTimeValues"][i]["time"], end_time=holiday_dict["listOfTimeValues"][i+1]["time"], occupancy=holiday_dict["listOfTimeValues"][i]["value"].value))

	def to_bacpypes(self):
		calendar_entry = None
		if len(self.start_date) == 4:
			if self.duration == 1:
				calendar_entry = CalendarEntry(date=self.start_date)
			elif self.duration > 1:
				calendar_entry = CalendarEntry(dateRange=DateRange(startDate=self.start_date, endDate=self.date_plus(self.start_date, self.duration, self.year)))
		elif len(self.start_date) == 3:
			calendar_entry = CalendarEntry(weekNDay=bytes(self.start_date))

		events = []
		for event in self.events:
			events.append(TimeValue(time=event.start_time, value=Integer(event.occupancy)))
			events.append(TimeValue(time=event.end_time, value=Integer(1)))
			# events.append(TimeValue(time=event.end_time, value=Null()))

		return SpecialEvent(period=SpecialEventPeriod(calendarEntry = calendar_entry), listOfTimeValues=events, eventPriority=1)

	def to_dict(self):
		result = \
		{
			"start_time": self["start_time"],
			"duration": self["duration"],
			"events": []
		}

		for event in self.events:
			result["events"].append(event.to_dict())

		return result

	def from_dict(self, dict_contents):
		self["start_date"] = dict_contents["start_date"]
		if len(self["start_date"]) == 3:
			self["duration"] = 1
		else:
			self["duration"] = dict_contents["duration"]

		if "year" in dict_contents and dict_contents["year"] != 1899:
			self.year = dict_contents["year"]
			self.for_all_years = False

		self.events = []
		for event_dict in dict_contents["events"]:
			self.events.append(Event(start_time=event_dict["start_time"], end_time=event_dict["end_time"], occupancy=event_dict["occupancy"]))

class HolidaySchedule:
	def __init__(self, schedule = None):
		self.holidays = []

		if isinstance(schedule, list) and len(schedule) != 0 and isinstance(schedule[0], dict):
			self.from_dict(schedule)
		elif schedule != None:
			self.from_bacpypes(schedule)

	def __len__(self):
		return len(self.holidays)

	def __eq__(self, holiday_schedule):
		return str(self) == str(holiday_schedule)
		# return self.to_dict() == holiday_schedule.to_dict()

	def from_bacpypes(self, schedule):
		self.holidays = []

		for holiday in schedule:
			self.holidays.append(Holiday(holiday))

	def to_bacpypes(self):
		holiday_schedule = []
		for holiday in self.holidays:
			holiday_schedule.append(holiday.to_bacpypes())
		return ArrayOf(SpecialEvent)(holiday_schedule)

	def __repr__(self):
		if len(self.holidays) == 0:
			return "No holidays."
			
		result = ""
		for i in range(len(self.holidays)):
			if i != len(self.holidays)-1:
				result += self.holidays[i].to_string() + "\n\n"
			else:
				result += self.holidays[i].to_string()

		return result

	def __getitem__(self, i):
		return self.holidays[i]

	def __setitem__(self, i, value):
		self.holidays[i] == value

	def setyear(self, year):
		self.year = year

	def add_holiday(self, holiday):
		self.holidays.append(copy.deepcopy(holiday))

	def delete_holiday(self, i):
		self.holidays.pop(i)

	def clear(self):
		self.holidays = []

	def to_dict(self):
		result = []
		for holiday in self.holidays:
			result.append(holiday.to_dict())

		return result

	def from_dict(self, dict_contents):
		self.holidays = []
		for holiday_dict in dict_contents:
			self.holidays.append(Holiday(holiday_dict))

	def empty(self):
		return (len(self.holidays) == 0)

class Weekday:
	def __init__(self, weekday = None):
		self.events = []
		if isinstance(weekday, list):
			self.from_dict(weekday)
		elif weekday != None:
			self.from_bacpypes(weekday)

	def __eq__(self, weekday):
		return (str(self) == str(weekday))
		# return (self.to_dict() == weekday.to_dict())

	def __len__(self):
		return len(self.events)

	def add_event(self, event):
		self.events.append(copy.deepcopy(event))

	def delete_event(self, i):
		self.events.pop(i)

	def clear(self):
		self.events = []

	def from_bacpypes(self, weekday):
		day = weekday.dict_contents()["daySchedule"]
		
		for j in range(len(day)):
			if not isinstance(day[j]["value"], Null) and day[j]["value"].value != 1:
				if j == len(day)-1:
					self.events.append(Event(start_time = day[j]["time"], end_time = (0,0,0,0), occupancy = day[j]["value"].value))
				else:
					self.events.append(Event(start_time = day[j]["time"], end_time = day[j+1]["time"], occupancy = day[j]["value"].value))

	def to_bacpypes(self):
		day_schedule = []
		for event in self.events:
			day_schedule.append(TimeValue(time=event.start_time, value=Integer(event.occupancy)))
			day_schedule.append(TimeValue(time=event.end_time, value=Integer(1)))
			# day_schedule.append(TimeValue(time=event.end_time, value=Null()))
	
		return DailySchedule(daySchedule=day_schedule)

	def __repr__(self):
		if len(self.events) == 0:
			return "    Unoccupied: 00:00 to 00:00\n"

		result = ""
		for i in range(len(self.events)):
			if i != len(self.events):
				result += "    " + str(self.events[i]) + "\n"
			else:
				result += "    " + str(self.events[i])

		return result

	def __getitem__(self, i):
		return self.events[i]

	def __setitem__(self, i, value):
		self.events[i] = value

	def to_dict(self):
		result = []
		for event in self.events:
			result.append(event.to_dict())

		return result

	def from_dict(self, dict_contents):
		self.events = []
		for event in dict_contents:
			self.events.append(Event(start_time=event["start_time"], end_time=event["end_time"], occupancy=event["occupancy"]))

	def empty(self):
		return (len(self.events) == 0)

class WeeklySchedule:
	def __init__(self, schedule = None):
		self.week_days = []

		if isinstance(schedule, dict):
			self.from_dict(schedule)
		elif schedule != None:
			self.from_bacpypes(schedule)
		else:
			for i in range(7):
				self.week_days.append(Weekday())

	def __len__(self):
		return 7

	def __eq__(self, weekly_schedule):
		return (str(self) == str(weekly_schedule))

	def day_name(self, i):
		if i == 1:
			return "Monday"
		elif i == 2:
			return "Tuesday"
		elif i == 3:
			return "Wednesday"
		elif i == 4:
			return "Thursday"
		elif i == 5:
			return "Friday"
		elif i == 6:
			return "Saturday"
		elif i == 7:
			return "Sunday"

	def day_id(self, day):
		if isinstance(day, str):
			day = day[0:2].lower()
			if 'm' in day:
				day = 1
			elif 'tu' in day:
				day = 2
			elif 'w' in day:
				day = 3
			elif 'th' in day:
				day = 4
			elif 'fr' in day:
				day = 5
			elif 'sa' in day:
				day = 6
			elif 'su' in day:
				day = 7
			else:
				raise KeyError(day)
		day -= 1

		return day

	def __getitem__(self, day):
		return self.week_days[self.day_id(day)]

	def clear(self):
		for i in range(7):
			self.week_days[i].clear()

	def __repr__(self):
		result = ""
		for i in range(1, 8):
			result += self.day_name(i) + ":\n"
			result += str(self.week_days[i-1])
			result += "\n"
		return result

	def to_bacpypes(self):
		weekly_schedule = []
		for i in range(1, 8):
			weekly_schedule.append(self.week_days[i-1].to_bacpypes())
		formatted_schedule = ArrayOf(DailySchedule)(weekly_schedule)
		return formatted_schedule

	def from_bacpypes(self, schedule):
		self.week_days = []
		for i in range(1, 8):
			self.week_days.append(Weekday(schedule[i-1]))

	def to_dict(self):
		result = {}

		for i in range(7):
			result[self.day_name(i+1)] = self.week_days[i].to_dict()

		return result

	def from_dict(self, dict_contents):
		self.week_days = []
		for i in range(7):
			self.week_days.append(Weekday(dict_contents[self.day_name(i+1)]))

	def empty(self):
		for week_ady in self.week_days:
			if not week_ady.empty():
				return False
		return True

if __name__ == "__main__":
	holiday_schedule = HolidaySchedule()
	holiday_schedule.from_dict(\
		[
			{
				"start_date": (1, 1),
				"duration": 2,
				"events":
				[
					{
						"start_time": "00:00",
						"end_time": "6:00",
						"occupancy": "Occupied"
					},

					{
						"start_time": "07:00",
						"end_time": "8:00",
						"occupancy": "Standby"
					},
				]
			},

			{
				"start_date": (3, 2, 6),
				"duration": 1,
				"events":
				[
					{
						"start_time": "00:00",
						"end_time": "6:00",
						"occupancy": "Occupied"
					},

					{
						"start_time": "07:00",
						"end_time": "8:00",
						"occupancy": "Standby"
					}
				]
			}
		]
	)
	print(holiday_schedule.to_dict())
