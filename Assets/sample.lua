



mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0)
glass = gr.material({0.0, 0.0, 0.0}, {0.5, 0.5, 0.5}, 25, 1.0)
copper = gr.material({0.82, 0.41, 0.12}, {1.0, 0.7, 0.3}, 5, 1.0)
sun = gr.material({0.9, 0.2, 0.0}, {1.0, 0.7, 0.3}, 5, 0.8)
sun2 = gr.material({1.0, 0.0, 0.0}, {1.0, 0.7, 0.3}, 5, 0.8)
blue = gr.material({0.4, 0.3, 0.6}, {1.0, 0.7, 0.3}, 60, 1.0)
purple = gr.material({0.09, 0.42, 0.75}, {1.0, 0.7, 0.3}, 60, 1.0)
white = gr.material({1, 1, 1}, {0.5, 0.7, 0.3}, 60, 1.0)
scene_root = gr.node('root')


scene_root = gr.node('root')

s3 = gr.nh_sphere('s3', {0, -100, -1460}, 120)
scene_root:add_child(s3)
s3:set_material(sun2)



b1 = gr.nh_box('b1', {-200, -225, 100}, 100)
scene_root:add_child(b1)
b1:set_material(glass)

s4 = gr.nh_sphere('s4', {-150, -085, 150}, 30)
scene_root:add_child(s4)
s4:set_material(glass)

s5 = gr.nh_sphere('s5', {-220, -100, 150}, 20)
scene_root:add_child(s5)
s5:set_material(white)

s6 = gr.nh_sphere('s6', {-260, -120, 130}, 20)
scene_root:add_child(s6)
s6:set_material(copper)


-- A small stellated dodecahedron.

steldodec = gr.mesh( 'dodec', 'smstdodeca.obj' )
steldodec:set_material(glass)
scene_root:add_child(steldodec)
steldodec:translate(100, -310, 240)

m1 = gr.mesh( 'cylinder', 'cylinder.obj' )
m1:set_material(purple)
scene_root:add_child(m1)
m1:scale(50, 50, 50)
m1:translate(150, -150, 0)

m2 = gr.mesh( 'icosa', 'icosa.obj' )
m2:set_material(glass)
scene_root:add_child(m2)
m2:scale(100, 150, 100)
m2:translate(-300, -250, -100)

m3 = gr.mesh( 'dodeca', 'dodeca.obj' )
m3:set_material(blue)
scene_root:add_child(m3)
m3:scale(50, 50, 50)
m3:translate(200, -200, -120)




-- the floor
plane = gr.mesh( 'plane', 'plane.obj' )
scene_root:add_child(plane)
plane:set_material(glass)
plane:scale(3000, 3000, 3000)
plane:translate(0, -200, 0)


white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
magenta_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'sample.png', 1080, 1080,
	  {0, -180, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, magenta_light})
