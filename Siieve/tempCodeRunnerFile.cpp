template <typename I, typename N>
requires(std::random_access_iterator<I> && std::integral<N>)
void sift0(I first, N n)
{
    std::fill(first, first + n, true);
    N i = 0;
    N index_square = 3;
    N factor = 3; // Starting factor for marking multiples
    while (index_square < n)
    {
        if (first[i])
        {
            mark_sieve(first + index_square, first + n, factor);
        }
        ++i;
        // Computing next index_square and factor
        index_square = 2 * i * (i + 3) + 3;
        factor += 2;
    }
}