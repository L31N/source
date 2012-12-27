
Command table:
    f-commands:     --> es wird nach text als ganze zeile gesucht und verarbeitet
        text[?replace]argument  --> text wird durch argument ersetzt
        text[?delete]           --> text wird geloescht
        text[?attach]argument   --> an text wird argument angehaengt

    x-commands:     --> es wird nach text als fragment gesucht, jedoch auch wie bei f-commands mit der ganzen zeile operiert
        text[&replace]argument  --> die ganze zeile, welche text enthaelt wird durch argument ersetzt
        text[&delete]           --> die ganze zeile, welche text enthaelt wird geloescht
        text[&attach]argument   --> an die zeile, welche text enthaelt wird argument angehaengt

    y-commands:     --> es wird nach text als fragment gesucht und nur mit diesem fragment operiert
        text[%replace]argument  --> text wird durch argument ersetzt
        text[%delete]           --> text wird aus der zeile herausgeloescht
        text[%attach]argument   --> argument wird innerhalt der zeile nachfolgend an text angehaengt

    z-commands:     --> es wird nach text als fragment gesucht, der nachfolgende text wird dann an das zeilenende der naechsten zeile angehaengt, welche argument enthaelt (bei attach)
        (text[*replace]argument)--> not working yet
        (text[*delete])         --> not working yet
        text[*attach]argument   --> inhalt, der text nachfolgt, wird an die nächste zeile die "G00" oder "G01" enthaelt angehaengt

Die Commands werden von f nach z abgearbeitet und dementsprechend priorisiert ...

please report problems and bugs to: daniel.belles@web.de
