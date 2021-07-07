import re
import os

pattern = re.compile(r"static\s+void\s+main\s*\(")

def delete_comments(content):
	while True:
		pos_start = content.find("/*")
		if pos_start == -1:
			break
		pos_end = content.find("*/", pos_start+2)
		if pos_end == -1:
			pos_end = len(content)-2
		pos_end += 2
		content = content[:pos_start] + content[pos_end:]

	while True:
		pos_start = content.find("//")
		if pos_start == -1:
			break
		pos_end = content.find("\n", pos_start+2)
		if pos_end == -1:
			pos_end = len(content)
		content = content[:pos_start] + content[pos_end:]

	return content

def is_main_class(file_name):
	content = open(file_name).read()
	content = delete_comments(content)
	return (pattern.search(content) != None)

def extname(file_name):
	basename = os.path.basename(file_name)
	pos_point = basename.rfind(".")
	if pos_point == -1:
		return ""

	return basename[pos_point+1:]

def all_java():
	all_files = []
	for root, dirs, files in os.walk(os.path.abspath(".")):
		for name in files:
			full_path = os.path.join(root, name)
			if extname(full_path) == "java":
				all_files.append(full_path)
	return all_files

for file in all_java():
	if is_main_class(file):
		relpath = os.path.relpath(file).replace("\\", "/")
		pos_slash = relpath.find("/")
		pos_point = relpath.rfind(".")
		relpath = relpath[pos_slash+1:pos_point]
		print(relpath, flush=True)
		break