using LinearAlgebra

function classic_(A, B, C)
    len = size(A, 1)
    for i in 1:len
        for j in 1:len
            for k in 1:len
                C[i, j] += (A[i, k] * B[k, j])
            end
        end
    end
    return C
end

FLOPS = 0
function multiply_rec_(A, B, C)
    if size(A, 1) == 2
        classic_(A, B, C)
        global FLOPS += 8
        return C
    end
    len = (size(A, 1) ÷ 2)
    a11 = [1:len, 1:len]
    a12 = [1:len, len+1:len*2]
    a21 = [len+1:len*2, 1:len]
    a22 = [len+1:len*2, len+1:len*2]

    multiply_rec_(
        A[a11...],
        B[a11...],
        @view C[a11...]
    )
    multiply_rec_(
        A[a12...],
        B[a21...],
        @view C[a11...]
    )

    multiply_rec_(
        A[a11...],
        B[a12...],
        @view C[a12...]
    )
    multiply_rec_(
        A[a12...],
        B[a22...],
        @view C[a12...]
    )

    multiply_rec_(
        A[a21...],
        B[a11...],
        @view C[a21...]
    )
    multiply_rec_(
        A[a22...],
        B[a21...],
        @view C[a21...]
    )

    multiply_rec_(
        A[a21...],
        B[a12...],
        @view C[a22...]
    )
    multiply_rec_(
        A[a22...],
        B[a22...],
        @view C[a22...]
    )
end

function multiply_rec(A, B)
    C = zeros(size(A, 1), size(B, 2))
    multiply_rec_(A, B, C)
    return C
end

function inv2x2(A)
    detA = A[1, 1] * A[2, 2] - A[1, 2] * A[2, 1]
    invA = [
        A[2, 2] -A[1, 2]
        -A[2, 1] A[1, 1]
    ]
    return (1 / detA) * invA
end

function inv_rec(A, B)

    if size(A, 1) == 2
        B[:, :] = inv2x2(A)
        global FLOPS += 8
        return
    elseif size(A, 1) == 1
        B[1, 1] = 1 / A[1, 1]
        global FLOPS += 1
        return
    end

    len = size(A, 1) ÷ 2
    a11 = [1:len, 1:len]
    a12 = [1:len, len+1:len*2]
    a21 = [len+1:len*2, 1:len]
    a22 = [len+1:len*2, len+1:len*2]

    invA11 = my_inv(A[a11...])

    S22 = A[a22...] - multiply_rec(
        multiply_rec(A[a21...], invA11),
        A[a12...]
    )

    invS22 = my_inv(S22)
    Id = Matrix(1.0I, len, len)

    B[a11...] = multiply_rec(
        invA11,
        Id + multiply_rec(
            multiply_rec(
                multiply_rec(A[a12...], invS22),
                A[a21...]
            ),
            invA11
        )
    )
    global FLOPS += 2
    B[a12...] = multiply_rec(multiply_rec(-invA11, A[a12...]), invS22)
    B[a21...] = multiply_rec(multiply_rec(-invS22, A[a21...]), invA11)
    B[a22...] = invS22
end

function my_inv(A::Matrix)
    B = 1.0 * zeros(size(A, 1), size(A, 1))
    inv_rec(A, B)
    return B
end