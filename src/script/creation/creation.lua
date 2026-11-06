math.randomseed(os.time())

function random_range(low, high)
  return low + math.random() * (high - low)
end

function random_location_creation( size_x, size_y, offset_x, offset_y)
  local radius = 0
  local diameter = math.max( size_x, size_y)
  local radius = diameter / 2

  local new_offset_x = 0
  local new_offset_y = 0

  local location_x = ( random_range(-1, 2) * diameter) - radius
  local location_y = ( random_range(-1, 2) * diameter) - radius

  local teta = math.atan(location_y, location_x)

  new_offset_x = radius * math.cos(teta) * 2.0
  new_offset_y = radius * math.sin(teta) * 2.0

  local layout = Layout.new(offset_x + new_offset_x, offset_y + new_offset_y, 1, 1, 1, 1, 0, 0, 0)

  create_object( layout )
end