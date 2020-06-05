import pyNebula as neckPy

print("We are live with pyNebula!\n")

neckPy.addEnt("P1", 0)
neckPy.findEnt("P1")
neckPy.addComp("TransformComp")
neckPy.addComp("GraphicalComp")
neckPy.addCompVar("transform", "matrix")
neckPy.addCompVar("nickname", "PythonGuy")

print("pyNebula has reached it's end!\n")