
red3 = gr.material({1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 60, 0.3, 0, "Assets/textures/red3.png")
blue10 = gr.material({0.4, 0.3, 0.6}, {1.0, 1.0, 1.0}, 60, 0.3, 0, "Assets/textures/blue10.png")
glass = gr.material({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 60, 0.8, 1.4, "")
lens_glass = gr.material({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 100, 0, 1.3, "")
metal = gr.material({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 100, 1.2, 0, "")
cloth = gr.material({0.09, 0.8, 0.2}, {0.2, 0.4, 0.2}, 5, 0, 0, "Assets/textures/cloth.png")
wood = gr.material({1.00, 0.0, 1.0}, {0.8, 0.6, 0.2}, 60, 0.2, 0, "Assets/textures/wood2.png")

scene_root = gr.node('root')


redball = gr.nh_sphere('redball', {400, 81, 180}, 80)
scene_root:add_child(redball)
redball:set_material(red3)
-- s3:rotate('x', 90)


blueball = gr.nh_sphere('blueball', {-360, 81, 120}, 80)
scene_root:add_child(blueball)
blueball:set_material(blue10)
-- s4:rotate('y', 90)

glassball = gr.nh_sphere('glassball', {700, 81, 0}, 80)
scene_root:add_child(glassball)
glassball:set_material(glass)
-- s4:rotate('y', 90)

metalball = gr.nh_sphere('metalball', {150, 81, -150}, 80)
scene_root:add_child(metalball)
metalball:set_material(metal)
-- s4:rotate('y', 90)


-- the floor
plane = gr.mesh( 'plane', 'plane.obj' )
scene_root:add_child(plane)
plane:set_material(cloth)
plane:scale(3000, 300, 1500)
plane:translate(0, 0, 0)

-- side bar

left_bar = gr.mesh( 'left_bar', 'cube.obj' )
scene_root:add_child(left_bar)
left_bar:set_material(wood)
left_bar:scale(300, 150, 3000)
left_bar:translate(-3000, 0, -1500)

right_bar = gr.mesh( 'right_bar', 'cube.obj' )
scene_root:add_child(right_bar)
right_bar:set_material(wood)
right_bar:scale(300, 150, 3000)
right_bar:translate(3000, 0, -1500)

front_bar = gr.mesh( 'front_bar', 'cube.obj' )
scene_root:add_child(front_bar)
front_bar:set_material(wood)
front_bar:scale(6000, 150, 300)
front_bar:translate(-3150, 0, 1500)

back_bar = gr.mesh( 'back_bar', 'cube.obj' )
scene_root:add_child(back_bar)
back_bar:set_material(wood)
back_bar:scale(6000, 150, 300)
back_bar:translate(-3150, 0, -1500)

white_light = gr.light({0.0, 550.0, 0.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
yellow_light = gr.light({600.0, 550.0, 600.0}, {1.0, 0.84, 0.7}, {0, -1, 0})
yellow_light2 = gr.light({-600.0, 550.0, -600.0}, {1.0, 0.84, 0.7}, {0, -1, 0})
yellow_light3 = gr.light({-600.0, 550.0, 600.0}, {1.0, 0.84, 0.7}, {0, -1, 0})
yellow_light4 = gr.light({600.0, 550.0, -600.0}, {1.0, 0.84, 0.7}, {0, -1, 0})


gr.render(scene_root, 'sample.png', 1080, 1080,
	  {1800, 800, 0}, {0, 800, 0}, {0, 1, 0}, 100,
	  {0.3, 0.3, 0.3}, {white_light, yellow_light, yellow_light2, yellow_light3, yellow_light4})
