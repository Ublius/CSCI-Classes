switch (state) {
    case "stuck_ground":
        state = "rising"
        vsp = -move_speed
        alarm[0] = hang_time
        break
        
    case "hanging":
        state = "falling"
        vsp = move_speed
        alarm[0] = hang_time
        break
        
    case "stuck_ceiling":
        state = "falling"
        vsp = move_speed
        alarm[0] = hang_time
        break
        
    default:
        state = "falling"
        vsp = move_speed
        alarm[0] = hang_time
}