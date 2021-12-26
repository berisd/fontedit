#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "render.h"
#include "cmocka.h"

static void test_render_copy_point(void **state) {
    BRS_Point point = {.x = 100, .y = 200};
    BRS_Point *pointCopy = BRS_copyPoint(&point);
    assert_non_null(pointCopy);
    assert_ptr_not_equal(pointCopy, &point);
    assert_int_equal(pointCopy->x, 100);
    assert_int_equal(pointCopy->y, 200);
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest render_tests[] = {
            cmocka_unit_test(test_render_copy_point)
    };
    return cmocka_run_group_tests(render_tests, NULL, NULL);
}

