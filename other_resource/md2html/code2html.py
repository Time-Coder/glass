import sys
import os
import markdown

def cat(filename):
	file = open(filename, "rb")
	content = file.read()
	file.close()
	return content.decode("utf-8").replace("\r", "")
	try:
		return content.decode("utf-8").replace("\r", "")
	except:
		return content.decode("ISO-8859-1").replace("\r", "")

def extname(filename):
	basename = os.path.basename(filename)
	pos_point = basename.rfind(".")
	if pos_point == -1 or pos_point == len(basename)-1:
		return ""

	return basename[pos_point+1:]

input_filename = sys.argv[1]
template_filename = os.path.dirname(os.path.abspath(__file__)) + "/template_code.html"
template = cat(template_filename).replace("plugins/", os.path.dirname(os.path.abspath(__file__)).replace("\\", "/") + "/plugins_offline/")
code = cat(input_filename)
lang = extname(input_filename).lower()
mkdown = "```" + lang + "\n" + code + "\n```"
html = markdown.markdown(mkdown, extensions=['fenced_code'])

pos_title = template.find("<title>")
final_result = template[:pos_title+len("<title>")] + os.path.basename(input_filename) + template[pos_title+len("<title>"):]
pos_body = final_result.find("<body>")
final_result = final_result[:pos_body+len("<body>")] + html + final_result[pos_body+len("<body>"):]

output_file = open(input_filename + ".html", "w", encoding='utf-8')
output_file.write(final_result)
output_file.close()