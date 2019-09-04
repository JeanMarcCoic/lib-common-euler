#include <lib-common/core.h>
#include <lib-common/container.h>

/**** Problem desc
 *
 *
 *
 * By starting at the top of the triangle below and moving to adjacent numbers on the row below, the maximum total from top
 * to bottom is 23.
 *
 * 3
 * 7 4
 * 2 4 6
 * 8 5 9 3
 *
 * That is, 3 + 7 + 4 + 9 = 23.
 *
 * Find the maximum total from top to bottom of the triangle below:
 *
 * 75
 * 95 64
 * 17 47 82
 * 18 35 87 10
 * 20 04 82 47 65
 * 19 01 23 75 03 34
 * 88 02 77 73 07 63 67
 * 99 65 04 28 06 16 70 92
 * 41 41 26 56 83 40 80 70 33
 * 41 48 72 33 47 32 37 16 94 29
 * 53 71 44 65 25 43 91 52 97 51 14
 * 70 11 33 28 77 73 17 78 39 68 17 57
 * 91 71 52 38 17 14 91 43 58 50 27 29 48
 * 63 66 04 68 89 53 67 30 73 16 69 87 40 31
 * 04 62 98 27 23 09 70 98 73 93 38 53 60 04 23
 *
 * NOTE: As there are only 16384 routes, it is possible to solve this problem by trying every route. However, Problem 67, is
 * the same challenge with a triangle containing one-hundred rows; it cannot be solved by brute force, and requires a clever
 * method! ;o)
 *
 *
 */

typedef struct node_t {
    uint64_t val;
    uint64_t max;
} node_t;

qvector_t(node, node_t);

qvector_t(rows, qv_t(node) *);

static int load_triangle(qv_t(rows) *rows)
{
    sb_t sb;
    pstream_t ps;

    sb_init(&sb);

    if (sb_read_file(&sb, "triangle.txt") < 0) {
        e_fatal("failed to read triangle.txt: %m");
    }

    ps = ps_initsb(&sb);

    while (ps_len(&ps) > 0) {
        pstream_t row;
        qv_t(node) *numbers;

        RETHROW(ps_get_ps_chr_and_skip(&ps, '\n', &row));

        numbers = qv_new(node);
        while (ps_len(&row) > 0) {
            uint64_t num = ps_getlli(&row);
            node_t node = { .val = num };

            qv_append(numbers, node);
            ps_skip(&row, 1);
        }
        qv_append(rows, numbers);
    }

    sb_wipe(&sb);

    return 0;
}

static void triangle_dump(qv_t(rows) *rows)
{
    for (int i = 0; i < rows->len; i++) {
        qv_t(node) *row = rows->tab[i];

        for (int j = 0; j < row->len; j++) {
            printf("%02ju ", row->tab[j].val);
        }
        printf("\n");
    }
}

static void triangle_max_compute(qv_t(rows) *rows)
{
    qv_t(node) *row = rows->tab[rows->len - 1];
    for (int i = 0; i < row->len; i++) {
        row->tab[i].max = row->tab[i].val;
    }
    for (int i = rows->len - 2; i >= 0; i--) {
        for (int j = 0; j < rows->tab[i]->len; j++) {
            rows->tab[i]->tab[j].max = rows->tab[i]->tab[j].val + MAX(rows->tab[i + 1]->tab[j].max, rows->tab[i + 1]->tab[j + 1].max);
        }
    }
}

int main(int argc, char **argv)
{
    qv_t(rows) rows;

    qv_init(&rows);

    load_triangle(&rows);

    triangle_dump(&rows);

    triangle_max_compute(&rows);

    e_info("max: %ju", rows.tab[0]->tab[0].max);

    qv_deep_wipe(&rows, qv_delete);

    return 0;
}
