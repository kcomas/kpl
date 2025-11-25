
# Control Flow

## If

condition ? statement or { lambda }

value :: condition ? {
    statement or { lambda } then
    statement or { lambda } else
}

value :: ? {
    condition ? statement or { lambda } then
    ...
    statement or { lambda } else
}

## Loop

condition @ statement or { lambda }

{ variable; condition; each } @ statement or { lambda }

## Match

value :: matchable # {
    var\`symbol { lambda }
    ...
}
