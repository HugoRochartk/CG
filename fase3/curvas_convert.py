






from dist_convert import convert_dist
from math import cos
import numpy
from pprint import pprint



def curvas_convert():
	dic = convert_dist()
	aux = cos(numpy.pi/4)
	tabbing = "\t\t\t"

	#           planeta : tempo que demora a rodar sobre si mesmo (em segundos), tempo que demora a dar a volta ao sol (em segundos) 300-76 
	info_sec = {"Mercúrio": (5034000, 7600000000), 
				"Vénus": (20952000, 19400000000),
				"Terra": (86400, 31557600),
				"Marte": (88640, 59354000),
				"Júpiter": (35040, 374355000),
				"Saturno": (39600, 929292000),
				"Urano": (62208, 2651370000),
				"Netuno": (58848, 5204420000),
				"Sol": (2196000, 0)
				}

	for planet, (dist_convert, dm_convert) in dic.items():
		if planet != "Sol" and planet != "Lua":
			str_res = f"{planet}:\n"
			str_res += f"<translate time = \"{round((info_sec[planet][1])*300/7600000000,2)}\" align=\"true\">\n"
			str_res += f"{tabbing}<point x = \"{dist_convert}\" y = \"0\" z = \"0\" />\n{tabbing}<point x = \"{round(dist_convert * aux,2)}\" y = \"0\" z = \"{round(-dist_convert * aux,2)}\" />\n{tabbing}<point x = \"0\" y = \"0\" z = \"-{dist_convert}\" />\n"
			str_res += f"{tabbing}<point x = \"{round(-dist_convert * aux)}\" y = \"0\" z = \"{round(-dist_convert * aux)}\" />\n{tabbing}<point x = \"{-dist_convert}\" y = \"0\" z = \"0\" />\n{tabbing}<point x = \"{round(-dist_convert * aux)}\" y = \"0\" z = \"{round(dist_convert * aux,2)}\" />\n"
			str_res += f"{tabbing}<point x = \"0\" y = \"0\" z = \"{dist_convert}\" />\n{tabbing}<point x = \"{round(dist_convert * aux,2)}\" y = \"0\" z = \"{round(dist_convert * aux,2)}\" />\n\n"
			str_res += f"<rotate time=\"{round((info_sec[planet][0]*200)/5034000,2)}\" x=\"0\" y=\"1\" z=\"0\" />\n\n\n"
			print(str_res)
		if planet == "Sol":
			str_res = f"{planet}:\n"
			str_res += f"<rotate time=\"{round((info_sec[planet][0]*200)/5034000,2)}\" x=\"0\" y=\"1\" z=\"0\" />\n\n\n"
			print(str_res)



curvas_convert()

