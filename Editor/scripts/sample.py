import Component_Modifier
from KeyCode import Key
def OnCreate() :
    print('Created')

def OnUpdate(ts) :
    print('Time is ' + str(ts))
    
    speed = float(0.5)
    x = float(0.0)
    y = float(0.0)
    if Component_Modifier.GetKeyPress(Key.S.value):
        y = -1.0
    elif Component_Modifier.GetKeyPress(Key.A.value):
        x = -1.0
    elif Component_Modifier.GetKeyPress(Key.W.value):
        y = 1.0
    elif Component_Modifier.GetKeyPress(Key.D.value):
        x = 1.0
        
    x = x*speed
    y = y*speed
    Component_Modifier.ApplyForce(x, y)

