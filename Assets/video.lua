


red3 = gr.material({1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 60, 0.3, 0, "Assets/textures/red3.png")
blue10 = gr.material({0.4, 0.3, 0.6}, {1.0, 1.0, 1.0}, 60, 0.3, 0, "Assets/textures/blue10.png")
glass = gr.material({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 60, 0.8, 1.4, "")
metal = gr.material({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 100, 1.5, 0, "")
cloth = gr.material({0.09, 0.8, 0.2}, {0.2, 0.4, 0.2}, 5, 0, 0, "Assets/textures/cloth.png")

scene_root = gr.node('root')


scene_root = gr.node('root')

s3 = gr.nh_sphere('redball', {100, 81, 100}, 80)
scene_root:add_child(s3)
s3:set_material(red3)
-- s3:rotate('x', 90)

s4 = gr.nh_sphere('blueball', {-120, 81, -400}, 80)
scene_root:add_child(s4)
s4:set_material(blue10)
-- s4:rotate('y', 90)

s5 = gr.nh_sphere('glassball', {-250, 81, 200}, 80)
scene_root:add_child(s5)
s5:set_material(glass)
-- s4:rotate('y', 90)

s6 = gr.nh_sphere('metalball', {350, 81, 250}, 80)
scene_root:add_child(s6)
s6:set_material(metal)
-- s4:rotate('y', 90)



-- the floor
plane = gr.mesh( 'plane', 'plane.obj' )
scene_root:add_child(plane)
plane:set_material(cloth)
plane:scale(3000, 3000, 3000)
plane:translate(0, 0, 0)

white_light = gr.light({-400.0, 350.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
yellow_light = gr.light({400.0, 350.0, 400.0}, {1.0, 0.84, 0.7}, {0, -1, 0})
yellow_light2 = gr.light({-400.0, 350.0, -400.0}, {1.0, 0.84, 0.7}, {0, -1, 0})
yellow_light3 = gr.light({-400.0, 350.0, 400.0}, {1.0, 0.84, 0.7}, {0, -1, 0})
yellow_light4 = gr.light({400.0, 350.0, -400.0}, {1.0, 0.84, 0.7}, {0, -1, 0})


gr.render(scene_root, 'sample.png', 1080, 1080,
	  {-700, 100, 700}, {0, 150, 0}, {0, 1, 0}, 104,
	  {0.3, 0.3, 0.3}, {white_light, yellow_light, yellow_light2, yellow_light3, yellow_light4})
