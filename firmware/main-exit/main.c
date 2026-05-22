/*
 * Return exit code 3 immediately so startup code reports
 * that main exited instead of idling or handling an exception.
 */
int main(void)
{
    return 3;
}
