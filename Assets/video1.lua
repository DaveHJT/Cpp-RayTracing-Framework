

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0, "")
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 0, "")
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0, "")
glass = gr.material({0.0, 0.0, 0.0}, {0.5, 0.5, 0.5}, 25, 1.0, "")
copper = gr.material({0.82, 0.41, 0.12}, {1.0, 0.7, 0.3}, 5, 1.0, "")
sun = gr.material({0.9, 0.2, 0.0}, {1.0, 0.7, 0.3}, 5, 0.8, "")
red3 = gr.material({1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 60, 1.0, "Assets/textures/red3.png")
blue10 = gr.material({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 60, 1.0, "")
green = gr.material({0.09, 0.0, 0.0}, {0.2, 0.4, 0.2}, 5, 0.0, "Assets/textures/cloth.png")
white = gr.material({1, 1, 1}, {0.5, 0.7, 0.3}, 60, 1.0, "")
scene_root = gr.node('root')


s4 = gr.nh_sphere('blueball', {-120, 80, 300}, 80)
scene_root:add_child(s4)
s4:set_material(blue10)
-- s4:rotate('y', 90)


s5 = gr.nh_sphere('blueball2', {40, 80, 300}, 80)
scene_root:add_child(s5)
s5:set_material(blue10)
-- s4:rotate('y', 90)



-- the floor
plane = gr.mesh( 'plane', 'plane.obj' )
scene_root:add_child(plane)
plane:set_material(green)
plane:scale(3000, 3000, 3000)
plane:translate(0, 0, 0)

white_light = gr.light({-400.0, 350.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
yellow_light = gr.light({400.0, 350.0, 400.0}, {1.0, 0.84, 0.7}, {0, -1, 0})
yellow_light2 = gr.light({-400.0, 350.0, -400.0}, {1.0, 0.84, 0.7}, {0, -1, 0})
yellow_light3 = gr.light({-400.0, 350.0, 400.0}, {1.0, 0.84, 0.7}, {0, -1, 0})
yellow_light4 = gr.light({400.0, 350.0, -400.0}, {1.0, 0.84, 0.7}, {0, -1, 0})


gr.render(scene_root, 'sample.png', 500, 500,
	  {-700, 100, 700}, {0, 150, 0}, {0, 1, 0}, 45,
	  {0.3, 0.3, 0.3}, {white_light, yellow_light, yellow_light2, yellow_light3, yellow_light4})
