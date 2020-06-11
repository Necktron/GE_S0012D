import pyNebula as neckPy
import json

print("We are live with JSON - 2!\n")

with open('../../../nebula-example-master/code/JSONONE.json') as path:
    data = json.load(path)

print(data)

length = len(data);

print(length)

for x in data:
    print(x["entName"])

    entName = x['entName']
    loadout = x['Loadout']

    print(entName)
    print(loadout)

    neckPy.addEnt(entName, loadout)
    neckPy.Find(x[entName])

    if(x['T_instance'] == 'Y'):
        neckPy.addComp("TransformComp")

    if(x['T_instance'] == 'Y'):
        neckPy.addComp("TransformComp")

    for var in x['varLibrary']:
        print(x['varLibrary'][var])
        varName = x['varLibrary']
        varVal = x['varLibrary'][var]
        neckPy.addCompVar(varName, varVal)

#neckPy.addEnt("P1", 0)
#neckPy.findEnt("P1")
#neckPy.addComp("TransformComp")
#neckPy.addComp("GraphicalComp")
#neckPy.addCompVar("transform", "matrix")
#neckPy.addCompVar("nickname", "PythonGuy")

print("JSON - 2 has reached it's end!\n")
