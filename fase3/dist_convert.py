from pprint import pprint

     
          
def convert_dist():
    far = 2800
    res = {}

    #            planeta | (dist ao sol em UA, diametro em km)
    info = {       'Sol': (0, 1391000),
               'Mercúrio': (0.39, 4879),
               'Vénus': (0.72, 12104),
               'Terra': (1.00, 12742),
               'Lua': (1.00, 3476),
               'Marte': (1.52, 6779),
               'Júpiter': (5.20, 139822),
               'Saturno': (9.58, 116460),
               'Urano': (19.18, 50724),
               'Netuno': (30.07, 49244)
       }
   
    
    for planeta in info:
        dist_UA = info[planeta][0]
        dmt = info[planeta][1]
        dist_convert = round((dist_UA * far) / 30.07, 2) #regra de 3 simples
        dmt_convert = round((dmt * 0.35) / 4879, 2) #regra de 3 simples
        res[planeta] = (dist_convert, dmt_convert)

    pprint(res)
    print("\n")
    
    return res
        


