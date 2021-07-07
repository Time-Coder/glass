from common import *

if not os.path.isfile(input_filename):
	print("Please give a markdown file name as input.")
	exit()

if extname(input_filename) != "md":
	print("Wrong file extention: ." + extname(input_filename) + ". Only .md extention is supported.")
	exit()

output_filename = os.path.dirname(os.path.abspath(input_filename)) + "/" + purename(input_filename) + ".html"

print("Compiling " + input_filename)
html = markdown.markdown(cat(input_filename), extensions=[MathExtension(enable_dollar_delimiter=True), 'pymdownx.superfences', 'tables'])
title, index, content = generate_index(html)
if title == "":
	title = purename(input_filename)

pos_title = template.find("<title>")
final_result = template[:pos_title+len("<title>")] + title + template[pos_title+len("<title>"):]
pos_index = final_result.find("<div id=\"content-index\">")
final_result = final_result[:pos_index+len("<div id=\"content-index\">")] + index + final_result[pos_index+len("<div id=\"content-index\">"):]
pos_content = final_result.find("<article class=\"markdown-body\">")
final_result = final_result[:pos_content+len("<article class=\"markdown-body\">")] + content + final_result[pos_content+len("<article class=\"markdown-body\">"):]

output_file = open(output_filename, "w", encoding='utf-8')
output_file.write(final_result)
output_file.close()