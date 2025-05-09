#include "rowtests.h"
#include "columntests.h"

int main(int argc, char *argv[])
{
    int status = 0;
    status |= QTest::qExec(new ColumnTests, argc, argv);
    status |= QTest::qExec(new RowTests, argc, argv);

    return status;
}
