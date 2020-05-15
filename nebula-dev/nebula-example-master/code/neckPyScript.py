import pyNebula as neckPy

print("We are live with pyNebula!")

neckPy.addEnt("P1", 0)
neckPy.findEnt("P1")
neckPy.addComp("TransformComp")
neckPy.addComp("GraphicalComp")
neckPy.addCompVarFloat("xVal", 2.0)
neckPy.addCompVarFloat("yVal", 5)
neckPy.addCompVarFloat("zVal", 3.0)
neckPy.addCompVar("transform", "matrix")
neckPy.addCompVar("nickname", "PythonGuy")
neckPy.setCompVar("nickname", "TwoHeadedPython")
neckPy.setCompVarFloat("xVal", 10.2)
neckPy.listOfEnt()
neckPy.getCompVar("nickname")
neckPy.getCompVar("xVal")
neckPy.getCompVar("entID")
neckPy.managerInit()
neckPy.getCompVar("GEID")

print("pyNebula has reached it's end!")