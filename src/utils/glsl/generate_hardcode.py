import os
import pathlib

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

out_file = open("_shaders.cpp", "w")
out_file.write("#include <string>\n")
out_file.write("\nusing namespace std;\n")
files = all_files(".")
shaders_content = ""
for file in files:
	var_name = file.replace("/", "_").replace("\\", "_").replace(".", "_").replace("glsl", "shader")
	content = open(file).read()
	lines = content.split("\n")
	needed_content = ""
	for line in lines:
		if line.strip() == "":
			continue
		needed_content += '"' + line.replace("\\", "\\\\").replace('"', '\\"') + '\\n"\n'
	out_file.write("string glass::" + var_name + " = \n")
	out_file.write(needed_content + ";\n\n")

out_file.close()

out_file = open("shaders.h", "w")
out_file.write("#include <string>\n")
out_file.write("namespace glass\n")
out_file.write("{\n")
files = all_files(".")
shaders_content = ""
for file in files:
	var_name = file.replace("/", "_").replace("\\", "_").replace(".", "_").replace("glsl", "shader")
	out_file.write("extern std::string " + var_name + ";\n")
out_file.write("};")
out_file.close()