import sys
import os
import shutil
import markdown
# from mdx_mathjax import MathJaxExtension
from mdx_math import MathExtension

def all_files(folder_name):
	result = []
	for root, dirs, files in os.walk(os.path.abspath(folder_name)):
		for name in files:
			result.append(os.path.join(root, name))

	return result
			
def modify_time(file_name):
	if os.path.isfile(file_name):
		return os.path.getmtime(file_name)
	else:
		return 0

def copy_file(src_file, dest_file):
	if modify_time(dest_file) >= modify_time(src_file):
		return
	if not os.path.isdir(os.path.dirname(os.path.abspath(dest_file))):
		os.makedirs(os.path.dirname(os.path.abspath(dest_file)))

	if not os.path.isfile(dest_file):
		print("Copying " + os.path.relpath(dest_file).replace("\\", "/"))
	else:
		print("Overwriting " + os.path.relpath(dest_file).replace("\\", "/"))

	shutil.copyfile(src_file, dest_file)

def copy_folder(src_folder, dest_folder):
	src_folder = os.path.abspath(src_folder)
	dest_folder = os.path.abspath(dest_folder)

	for file_name in all_files(src_folder):
		copy_file(file_name, file_name.replace(src_folder, dest_folder))

def cat(filename):
	file = open(filename, "rb")
	content = file.read()
	file.close()
	return content.decode("utf-8").replace("\r", "")
	try:
		return content.decode("utf-8").replace("\r", "")
	except:
		return content.decode("ISO-8859-1").replace("\r", "")

def delete_html_comments(content):
	pos_start = 0
	pos_end = 0
	while True:
		pos_start = content.find("<!--", pos_end+3)
		if pos_start == -1:
			break
		pos_end = content.find("-->", pos_start+4)
		if pos_end == -1:
			pos_end = len(content)-3
		content = content[:pos_start] + content[pos_end+3:]

	return content

def format_section(section_counter):
	section_number = ""
	for sec in section_counter:
		section_number += str(sec) + "."

	return section_number[:-1]


def get_li(section_map, i):
	result = "<li><div class=\"item-arrow\"></div><a href=\"#section-" + section_map[i][0].replace(".", "-") + "\">" + section_map[i][0] + "&nbsp;&nbsp;" + section_map[i][1] + "</a>"
	current_i = i
	i += 1

	result += "\n<ul class=\"index\">\n"
	while i < len(section_map) and len(section_map[i][0].split(".")) > len(section_map[current_i][0].split(".")):
		items = get_li(section_map, i)
		result += items[0]
		i = items[1]

	result += "</ul>\n"
	result += "</li>\n"
	result = result.replace("\n<ul class=\"index\">\n</ul>\n", "")

	return result, i
	
def format_index(section_map):
	result = "<ul class=\"index\">\n"
	i = 0
	while i < len(section_map):
		li, i = get_li(section_map, i)
		result += li
	result += "</ul>"
	return result

def generate_index(content):
	content = delete_html_comments(content)
	title = ""

	section_counter = []
	section_map = []

	pos_hi_start = content.find("<h1")
	pos_hi_content_end = 0
	if pos_hi_start != -1:
		content = content[:pos_hi_start+3] + " id=\"title\"" + content[pos_hi_start+3:]
		pos_hi_content_end = content.find("</h1>", pos_hi_start)

		pos_title_start = content.find(">", pos_hi_start) + 1
		title = content[pos_title_start:pos_hi_content_end]
	else:
		pos_hi_start = 0
	
	while True:
		pos_hi_start = content.find("<h", pos_hi_content_end)
		if pos_hi_start == -1:
			break
		if content[pos_hi_start+2] not in "123456" or content[pos_hi_start+3] not in ' \t>':
			pos_hi_content_end = pos_hi_start + 2
			continue
			
		pos_hi_end = content.find(">", pos_hi_start+3)
		pos_id = content.find("id", pos_hi_start+3, pos_hi_end)
		if pos_id != -1:
			pos_hi_content_end = pos_hi_end + 1
			continue

		i = int(content[pos_hi_start+2])
		while len(section_counter) < i:
			section_counter.append(0)
		section_counter = section_counter[:i]
		section_counter[i-1] += 1
		section_number = format_section(section_counter)
		content = content[:pos_hi_start+3] + " id=\"section-" + section_number.replace(".", "-") + "\" class=\"h-title\" " + content[pos_hi_start+3:]

		pos_hi_end = content.find(">", pos_hi_start+3)
		pos_hi_content_start = pos_hi_end+1
		pos_hi_content_end = content.find("</h" + str(content[pos_hi_start+2]) + ">", pos_hi_content_start)

		section_map.append((section_number, content[pos_hi_content_start:pos_hi_content_end]))

		content = content[:pos_hi_end+1] + section_number + "&nbsp;&nbsp&nbsp;" + content[pos_hi_end+1:]
		pos_hi_content_end = content.find("</h" + str(content[pos_hi_start+2]) + ">", pos_hi_content_start)

	index = format_index(section_map)
	return title, index, content

def purename(filename):
	basename = os.path.basename(filename)
	pos_point = basename.rfind(".")
	if pos_point == -1:
		return basename

	return basename[:pos_point]

def extname(filename):
	basename = os.path.basename(filename)
	pos_point = basename.rfind(".")
	if pos_point == -1 or pos_point == len(basename)-1:
		return ""

	return basename[pos_point+1:]

standalone = True
offline = None
input_filename = "."
for i in range(1, len(sys.argv)):
	if sys.argv[i] == "--release":
		standalone = False
	if sys.argv[i] == "--offline":
		offline = True
	if sys.argv[i][0] != '-':
		input_filename = sys.argv[i]

if offline == None:
	offline = standalone

if input_filename == "":
	print("Please give an input.")
	exit()

if not os.path.exists(input_filename):
	print(input_filename + " is not exist.")
	exit()

base_path = os.path.dirname(os.path.abspath(__file__))
template_filename = base_path + "/template_online.html"
if offline:
	template_filename = base_path + "/template_offline.html"

template = cat(template_filename)

if not standalone:
	if os.path.isfile(input_filename):
		if offline:
			copy_folder(base_path + "/plugins_offline", os.path.dirname(os.path.abspath(input_filename)) + "/plugins")
		else:
			copy_folder(base_path + "/plugins_online", os.path.dirname(os.path.abspath(input_filename)) + "/plugins")
	else:
		if offline:
			copy_folder(base_path + "/plugins_offline", os.path.abspath(input_filename) + "/plugins")
		else:
			copy_folder(base_path + "/plugins_online", os.path.abspath(input_filename) + "/plugins")
else:
	template = template.replace("plugins/", os.path.dirname(os.path.abspath(__file__)).replace("\\", "/") + "/plugins_offline/")