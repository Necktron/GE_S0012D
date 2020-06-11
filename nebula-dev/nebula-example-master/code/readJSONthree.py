import pyNebula as neckPy
import json

print("We are live with JSON - 3!\n")

with open('../../../nebula-example-master/code/JSONONE.json') as path:
    data = json.loads(path)

print(data)

length = len(data);

print(length)

for key, value in data.items():
    print(key, value)

#neckPy.addEnt("P1", 0)
#neckPy.findEnt("P1")
#neckPy.addComp("TransformComp")
#neckPy.addComp("GraphicalComp")
#neckPy.addCompVar("transform", "matrix")
#neckPy.addCompVar("nickname", "PythonGuy")

print("JSON - 3 has reached it's end!\n")
