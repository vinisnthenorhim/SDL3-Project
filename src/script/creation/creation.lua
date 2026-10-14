math.randomseed(os.time())



function random_location_creation( size_x, size_y )
  local x_half = size_x / 2
  local y_half = size_y / 2

  local location_x = (math.random() * size_x) - x_half
  local location_y = (math.random() * size_y) - y_half
  print("update")

  create_object(EntityType.Player, location_x, location_y)
  print("update")
end