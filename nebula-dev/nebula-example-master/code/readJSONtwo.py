import pyNebula as neckPy
import json

print("We are live with JSON - 2!\n")

with open('../../../nebula-example-master/code/JSONTWO.json') as path:
    data = json.load(path)

print(data)

for x in data:

    entName = x['entName']
    loadout = x['Loadout']

    neckPy.addEnt(str(entName), int(loadout))
    neckPy.findEnt(str(entName))

    if(x['T_instance'] == 'Y'):
        neckPy.addComp("TransformComp")

    if(x['G_instance'] == 'Y'):
        neckPy.addComp("GraphicalComp")

    for var in x['varLibrary']:
        varName = x['varLibrary']
        varVal = x['varLibrary'][var]

        if("nickname" in varName):
            neckPy.addCompVar("nickname", str(varVal))

        elif("xVal" in varName):
            neckPy.addCompVarFloat("xVal", float(varVal))

        elif("yVal" in varName):
            neckPy.addCompVarFloat("yVal", float(varVal))

        elif("zVal" in varName):
            neckPy.addCompVarFloat("zVal", float(varVal))