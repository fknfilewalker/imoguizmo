# ImOGuizmo
A simple header only interactive orientation gizmo for ImGui. 

## Usage
```c++
#include "imguizmo.hpp"

if(ImOGuizmo::drawGizmo(viewMatrix, { 0.0f, 0.0f }, 120.0f, pivotDistance, false))
{
	// in case of user interaction viewMatrix gets updated
}
```
Drag|Click
:-:|:-:
![drag_example](images/drag.gif)  |  ![click_example](images/click.gif)

[License (MIT)](https://github.com/fknfilewalker/imoguizmo/blob/main/LICENSE)