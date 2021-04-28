#include "scenes_view.h"

int main()
{
	ScenesView figure(800, 600, "Figure", Color(40, 41, 35));
	figure.show();

	return 0;
}

// todo:
// 1. change word coord to x-right y-up z-back // done
// 2. model view double click // done
// 3. image flip y on load // done
// 4. image only load once // done
// 5. layout continuesly memory // done
// 7. change callback method
// 8. layout extract from glsl not only location // done
// 9. bug: skybox is inverse // done
// 10. sphere generate method
// 11. add planets to Model
// 12. add pure elements to Material
// 13. normal map // done
// 14. parallax map // done
// 15. bump map
// 16. displacement map
// 17. uniform works just like std::map
// 18. layout works just like std::map
// 19. change double to float (102M runtime memory now!)
// 20. correct middle drag precision
// 21. hide model mat to private
// 22. add light methods
// 23. add light mat
// 24. Create Scenes Object