# ImOGuizmo
A simple C++11 header only interactive orientation gizmo for ImGui. 

## Usage
```c++
#include "imoguizmo.hpp"

// specify position and size of guizmo window
// optional: set distance to pivot (-> activates interaction)
// optional: show background
if(ImOGuizmo::drawGizmo(viewMatrix, { 0.0f, 0.0f }, 120.0f, pivotDistance, false))
{
	// in case of user interaction viewMatrix gets updated
}
```
Drag|Click
:-:|:-:
![drag_example](images/drag.gif)  |  ![click_example](images/click.gif)

[License (MIT)](https://github.com/fknfilewalker/imoguizmo/blob/main/LICENSE)