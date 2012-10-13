
To use this tool you have to create an config file in the same directory, as the runpath is... (normally the directory of the executable):
The config-file have to be the name: cnc.conf.

To write a config file... you can use any text editor ... (ascii)
Use '#' at the first character of a line to mark it as a comment.
Blanklines will be ignored
Use the following operators to give the application instructions, how it should convert the format ...

Available Commands / Operators:

    text[replace]argument        --> wenn text die ganze zeile darstellt, dann wird diese durch arument ersetzt
    text[delete]                 --> wenn text die ganze zeile darstellt, dann wird diese geloescht
    text[attach]argument         --> wenn text die ganze zeile darstellt, dann wird dieser argument angehaengt

    text[&replace]argument       --> wenn text in der zeile enthalten ist, wird die gesamte zeile durch argument ersetzt
    text[&delete]                --> wenn text in der zeile enthalten ist, wird die gesamte zeile geloescht
    text[&attach]argument        --> wenn text in der zeile enthalten ist, wird an die gesamte zeile argument angehaengt

    text[%replace]argument       --> wenn text in der zeile enthalten ist, wird text durch argument ersetzt (rest wird beibehalten)
    text[%delete]                --> wenn text in der zeile enthalten ist, wird nur text aus dieser geloescht
    text[%attach]argument        --> wenn text in der zeile enthalten ist, wird nach text argument angehaengt


The usage of the application is: "cncconvert INPUT_FILE_PATH OUTPUT_FILE_PATH"

Report bugs and problems to: "daniel.belles@web.de"
