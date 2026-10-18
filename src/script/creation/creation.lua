math.randomseed(os.time())



function random_location_creation( size_x, size_y )
  local x_half = size_x / 2
  local y_half = size_y / 2

  local location_x = (math.random() * size_x) - x_half
  local location_y = (math.random() * size_y) - y_half

  local layout = Layout.new(location_x, location_y, 1, 1, 1, 1, 0, 0, 0)

  create_object( layout )
end