
scene_root = gr.node('root')


scene_root = gr.node('root')



-- the floor


white_light = gr.light({-400.0, 350.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})



gr.render(scene_root, 'sample.png', 1080, 1080,
	  {0, 0, 0}, {-1, 0, 0}, {0, 1, 0}, 80,
	  {0.3, 0.3, 0.3}, {white_light})
