
function factorial (integer a, integer b) result (b)
    integer c
    a = a - 1
    if (a > 0) then
        b = b * factorial(a, b+1)
    end if
end function factorial

program example
    integer::c = 0, b=3
    c = factorial((1*2)*2+(b), 1)
    print* c
end program example


