# ICT SBA Knockout System

A Qt-based tournament management application for generating knockout brackets from Excel data. This system allows users to import participant lists from Excel files and visualize tournament brackets through an intuitive GUI interface.

## Features

- Import participant data from Excel (.xlsx) files using QXlsx library
- Generate visual knockout tournament brackets
- Interactive GUI for tournament management
- Input validation and error handling
- Warning system for invalid tournament configurations

## Project Structure

```
├── headers/        # C++ header files
├── QXlsx/          # Excel processing library (QXlsx)
├── src/            # Source code implementation
├── ui/             # Qt Designer interface files
├── KnockOutSystem.pro  # Qt project configuration
└── README.md
```

## Dependencies

- Qt 5 or later
- QXlsx library (included in repository)

## Build Instructions

1. Ensure Qt development environment is installed
2. Open terminal in project directory
3. Run:

```bash
qmake KnockOutSystem.pro
make
```

4. Execute the generated binary:

```bash
./KnockOutSystem
```

## Usage

1. Launch the application
2. Use the input interface to load participant data from Excel
3. Generate and view tournament brackets
4. Save/export tournament results as needed

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
