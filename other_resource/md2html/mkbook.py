from common import *

if not os.path.isdir(input_filename):
	print("Please give a folder name as input.")
	exit()

book_name = os.path.basename(os.path.abspath(input_filename))
output_filename = input_filename + "/" + book_name + ".html"

files = os.listdir(".")
files = sorted(files, key=os.path.abspath)

md_content = ""
n_valid_files = 0
for file_name in files:
	if os.path.isfile(file_name) and extname(file_name) == "md":
		print("Compiling " + file_name)
		md_content += cat(file_name) + "\n\n"
		n_valid_files += 1
		book_name = purename(file_name)

if n_valid_files == 0:
	print("Nothing to be done.")
	exit()

if n_valid_files == 1:
	output_filename = input_filename + "/" + book_name + ".html"

print("Linking " + output_filename)
html = markdown.markdown(md_content, extensions=[MathExtension(enable_dollar_delimiter=True), 'pymdownx.superfences', 'tables'])
title, index, content = generate_index(html)
if title == "":
	title = book_name
	
pos_title = template.find("<title>")
final_result = template[:pos_title+len("<title>")] + title + template[pos_title+len("<title>"):]
pos_index = final_result.find("<div id=\"content-index\">")
final_result = final_result[:pos_index+len("<div id=\"content-index\">")] + index + final_result[pos_index+len("<div id=\"content-index\">"):]
pos_content = final_result.find("<article class=\"markdown-body\">")
final_result = final_result[:pos_content+len("<article class=\"markdown-body\">")] + content + final_result[pos_content+len("<article class=\"markdown-body\">"):]

output_file = open(output_filename, "w", encoding='utf-8')
output_file.write(final_result)
output_file.close()