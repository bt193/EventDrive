enum Direction
{
    Forward,
    Backward
};

enum ExpectedVersion
{
    Any = -1, // Append As-is
    NoStream = -2, // No Events
    EmptyStream = -3 // Has Never Existed
};