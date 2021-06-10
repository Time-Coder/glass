import os
import pathlib
import copy

def delete_comments(content):
	while True:
		pos_start = content.find("/*")
		if pos_start == -1:
			break

		pos_end = content.find("*/", pos_start+2)
		if pos_end == -1:
			pos_end = len(content)-2

		content = content[:pos_start] + content[pos_end+2:]

	while True:
		pos_start = content.find("//")
		if pos_start == -1:
			break

		pos_end = content.find("\n", pos_start + 2)
		if pos_end == -1:
			pos_end = len(content)

		content = content[:pos_start] + content[pos_end:]

	return content

class Node:
	def __init__(self):
		self.content = ""
		self.in_edges = set()

def skip_space(content, pos):
	if pos < 0:
		return 0
	if pos >= len(content):
		return len(content)

	while pos < len(content):
		if content[pos] not in ' \t':
			break

		pos += 1

	return pos

def to_hard_code(filename):
	content = delete_comments(open(filename).read())
	lines = content.split("\n")
	name = filename.replace("/", "_").replace("\\", "_").replace(".", "_").replace("glsl", "shader")
	result = Node()

	for line in lines:
		line = line.rstrip()
		if line == "":
			continue

		pos_start = line.find("#include")
		if pos_start == -1:
			line = '"' + line.replace('\\', '\\\\').replace('"', '\\"') + '\\n"'
		else:
			pos_start = skip_space(line, pos_start+len("#include"))
			start_sign = line[pos_start]
			pos_start += 1
			if start_sign == '"':
				end_sign = '"'
			elif start_sign == '<':
				end_sign = '>'

			pos_end = line.find(end_sign, pos_start)
			if pos_end == -1:
				raise SyntexError("Error format in #include")
			file_name = line[pos_start:pos_end]
			file_name = os.path.relpath(os.path.dirname(filename) + "/" + file_name)
			var_name = file_name.replace("/", "_").replace("\\", "_").replace(".", "_").replace("glsl", "shader")
			line = '+ glass::' + var_name + ' +'
			result.in_edges.add(var_name)
			
		result.content += line + '\n'

	return result

def generate_hardcode_dict(folder_name):
	files = all_files(".")
	result = {}
	for file in files:
		var_name = file.replace("/", "_").replace("\\", "_").replace(".", "_").replace("glsl", "shader")
		result[var_name] = to_hard_code(file)
	return result

def extname(filename):
	return pathlib.Path(filename).suffix

def all_files(folder = "."):
	result = []
	files = os.listdir(folder)
	for file in files:
		file = folder + "/" + file
		if os.path.isfile(file) and extname(file) == ".glsl":
			if len(file)>=2  and (file[:2] == "./" or file[:2] == ".\\"):
				file = file[2:]
			result.append(file)
		elif os.path.isdir(file):
			result.extend(all_files(file))
	return result

result_dict = generate_hardcode_dict(".")
result_dict_copy = copy.deepcopy(result_dict)

order_list = []

while True:
	pop_key = None
	for key in result_dict:
		if len(result_dict[key].in_edges) == 0:
			order_list.append(key)
			for sub_key in result_dict:
				if key in result_dict[sub_key].in_edges:
					result_dict[sub_key].in_edges.remove(key)
			pop_key = key
			break

	if pop_key != None:
		result_dict.pop(pop_key)
	else:
		break

for key in result_dict:
	print(result_dict[key].in_edges)

out_file = open("../shaders.cpp", "w")
out_file.write("#include \"glass/utils/shaders.h\"\n")
out_file.write("\nusing namespace std;\n")
for var_name in order_list:
	out_file.write("string glass::" + var_name + " = \n")
	out_file.write(result_dict_copy[var_name].content + ";\n\n")

out_file.close()

out_file = open("../../../include/glass/utils/shaders.h", "w")
out_file.write("#ifndef __SHADERS_H__\n")
out_file.write("#define __SHADERS_H__\n")
out_file.write("#include <string>\n")
out_file.write("namespace glass\n")
out_file.write("{\n")
for var_name in order_list:
	out_file.write("extern std::string " + var_name + ";\n")
out_file.write("};\n")
out_file.write("#endif")
out_file.close()