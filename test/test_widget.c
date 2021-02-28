#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "widget/widget.h"
#include "cmocka.h"

static void test_widget_properties_create(void **state) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create("testid");
    assert_non_null(widgetProps);
}

static void test_widget_properties_copy(void **state) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create("testid");
    BRS_GUI_Widget_Properties *copy = BRS_GUI_Widget_Properties_copy(widgetProps);
    assert_non_null(copy);
    assert_ptr_not_equal(copy, widgetProps);
}

int main(void) {
    const struct CMUnitTest widget_tests[] = {
            cmocka_unit_test(test_widget_properties_create),
            cmocka_unit_test(test_widget_properties_copy)
    };
    return cmocka_run_group_tests(widget_tests, NULL, NULL);
}

