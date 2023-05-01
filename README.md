# ImOGuizmo
A simple C++11 header only interactive orientation gizmo for ImGui. 

## Usage
```c++
#include "imoguizmo.hpp"

// it is recommended to use a separate projection matrix since the values that work best
// can be very different from what works well with normal renderings
// aspect ratio should be 1 otherwise axes would scale differently
// e.g. with glm -> glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);

// specify position and size of guizmo window
// optional: set distance to pivot (-> activates interaction)
// optional: show background
if(ImOGuizmo::drawGizmo(viewMatrix, projMat, { 0.0f, 0.0f }, 120.0f, pivotDistance, false))
{
	// in case of user interaction viewMatrix gets updated
}
```
Drag|Click
:-:|:-:
![drag_example](images/drag.gif)  |  ![click_example](images/click.gif)

[License (MIT)](https://github.com/fknfilewalker/imoguizmo/blob/main/LICENSE)