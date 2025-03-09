[33mcommit bea8e686380ac7be018336eeaa15c35e93456a0d[m[33m ([m[1;36mHEAD -> [m[1;32mmaster[m[33m)[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun Jan 26 13:55:41 2025 +0100

    Böschung: Testprogramm geschrieben, Falschen Vergleich in LinieExtrudieren geändert

[33mcommit 5319ae6892153038353564edd6d27cc59adb4cab[m[33m ([m[1;31morigin/master[m[33m, [m[1;31morigin/HEAD[m[33m)[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sat Jan 25 15:13:45 2025 +0100

    Böschung extrudieren begonnen (8)
            EingabeMaske hinzugefügt (für double Eingaben)
            Routine in RUZmBIFrame zur Erstellung der Böschungen hinzugefügt
    
            Böschung werden noch nicht erzeugt => Fehlersuche erforderlich

[33mcommit 764b467d9901f368965fb77b14af775959f90704[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun Jan 19 10:28:49 2025 +0100

    Böschung extrudieren begonnen (7)

[33mcommit 2e6faf17f6d80bb1f8822285a9b0ecb3efee0baf[m
Author: Ansgar Rütten <ansgar.ruetten@gmx.de>
Date:   Fri Jan 17 09:32:07 2025 +0100

    Böschung extrudieren begonnen (6)

[33mcommit 770f983a0f02c13c18fe917a1d8f3c4a180e49fc[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Fri Jan 17 07:45:03 2025 +0100

    Böschung extrudieren begonnen (5); Dreiecke erzeugt, aber doppelte Linien! Linie::NeueLinie() muss ergänzt werden

[33mcommit d47cd5c9ec2db968d7dcb7bb08f28d8fe0719504[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sat Jan 11 14:44:09 2025 +0100

    Böschung extrudieren begonnen (4)

[33mcommit 0ca38afa55cffedc988a63319f8a8a513f89a9b1[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Mon Jan 6 17:13:21 2025 +0100

    Böschung extrudieren begonnen (3)

[33mcommit 8782588c24ac475d515bee40808ad7c66c4f8e68[m
Author: Ansgar <Ansgar.ruetten@gmx.de>
Date:   Sat Jan 4 17:04:43 2025 +0100

    Böschung extrudieren begonnen (2)

[33mcommit 0914ad91d5b58dae70a85a9a3c39089f59be77a0[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Thu Jan 2 15:28:02 2025 +0100

    Böschung extrudieren begonnen

[33mcommit 99779439a0f3665b2158ef1ae2097c4dd46c7e1b[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Mon Dec 30 21:09:50 2024 +0100

    Linie Extrudieren:
            Lotfusspunkt korrigiert (Projektionsachse)
            Zwei Dreiecke statt eines Vierecks erzeugen

[33mcommit b29692814e1e9c44996d48de783fc10715723722[m
Author: Ansgar <Ansgar.ruetten@gmx.de>
Date:   Thu Nov 7 23:13:47 2024 +0100

    Ermittlung Gefälle des Vierecks korrigiert

[33mcommit 3d3f9b68db21dfde3e7431f3dda94be3a266e633[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun Sep 22 14:27:56 2024 +0200

    Integrationsthread ohne gemeinsame Datenbereich

[33mcommit 809bb1b28d1f86ab9885ba5855fe657c54154a47[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun Sep 8 16:45:28 2024 +0200

    Iteration über Liste in HoehenkarteZeichnen jetzt über Listenelemente (ansonsten komischer Bug => weitere Untersuchung notwendig)
    Thread Hoehenkarte in die Klasse zurückgeholt

[33mcommit 8329aa35f026144a2c1834bd435411cfab625d05[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun Sep 8 10:44:04 2024 +0200

    Thread Integral implementiert
            Flächen werden zweimal integriert - warum???

[33mcommit db256c93193c0f865476add3ed042c5c727c9e7e[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Wed Sep 4 19:49:56 2024 +0200

    Absturz nach löschen des letzten Layers abgefangen.

[33mcommit 1c9d97b9ffabfd0f70056c5cab2414d7810c26ab[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Wed Sep 4 13:02:22 2024 +0200

    LayerDialoge.h und .cpp erstellt und alle Dialoge zur Layersteuerung ausgelagert

[33mcommit e287fe9a73c8a6e6c2cd5d09290fb947c9a1da2f[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sun Sep 1 22:11:30 2024 +0200

    Fehlerhaften include gefixt (MainFrame)
            ToDo: gethreadete Funktionen im Namen kennzeichnen

[33mcommit 051ddc0880e1c53a74ca0ece4709b53264f1429b[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sun Aug 25 12:28:45 2024 +0200

    thread_inf_vernetzen Fehler behoben (hat immer alle Linien gelöscht)

[33mcommit 968d59c6a28e77930f1f07b3b7db3d9578429500[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sun Aug 25 12:03:35 2024 +0200

    ThreadCtrl verallgemeinert
            ThreadCtrl nur noch zur Steuerung und Anzeige
            thread_info stellt alle vorgangsspezifischen Informationen zur Verfügung
    
            ToDo: LayerAuswahlDialog soll zur Anzeige jedes mal neu erstellt werden, damit Layer von außerhalb (Thread) gelöscht werden können

[33mcommit 1c62fb7af852e320af3d1cbe18acf5aad2e1ceb0[m
Merge: b4aa0dc 05f33a1
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sat Aug 24 00:13:41 2024 +0200

    Merge branch 'master' of https://github.com/RuebenBaer/RUZ

[33mcommit b4aa0dccbb426b713507a23ee5d3be514a19aec2[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Tue Aug 20 07:10:54 2024 +0200

    Thread: Vernetzen implementiert

[33mcommit 05f33a15ce41169940efc5a686a4702ca1035b35[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sat Aug 24 00:03:27 2024 +0200

    Thread: Vernetzen implementiert

[33mcommit 0857758d38e9afbee48635d37a672f67b410362a[m
Author: Ansgar Rütten <ansgar.ruetten@gmx.de>
Date:   Fri Aug 23 13:51:22 2024 +0200

    Thread: Vernetzen => part 3

[33mcommit df0c50387380b8a4bb27a3a44cbbafd8354761fe[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Fri Aug 23 08:05:18 2024 +0200

    Thread: Vernetzen => part 2

[33mcommit 2f4cb046c30d9900cb6b60464c9816acd5518abd[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Thu Aug 22 21:24:51 2024 +0200

    Thread: Vernetzen => part 2

[33mcommit e3ba9de79e465b1a14a0910f1ff49ccb3fde4325[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Tue Aug 20 07:10:54 2024 +0200

    Thread: Vernetzen => part 1

[33mcommit 672823afd54f6827f1a81b4216b7be3ddf6ea902[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Thu Aug 15 10:29:24 2024 +0200

    Ergebnisausgabe Integral angepasst

[33mcommit 909e9308c1eac824ab11f2a677917fc7d5fe93b9[m
Merge: 0c8c7c9 6f2256e
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Tue Aug 13 20:00:31 2024 +0200

    Merge branch 'master' of https://github.com/RuebenBaer/RUZ

[33mcommit 0c8c7c961168c3226e11816b864b0ef408f9d404[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Tue Aug 13 19:53:45 2024 +0200

    Volumenermittlung an neues Integral angepasst
    Farbgebung bei der Ergebnisausgabe geändert - Gradient hinzugefügt
    In ThreadControl OnEreaseBackground ergänzt um Flackern zu verhindern

[33mcommit 6f2256ebc33a16e0ed552439a5a461a4dc3d24b3[m[33m ([m[1;32mIntegral_bruteForce[m[33m)[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Tue Aug 13 19:53:45 2024 +0200

    Volumenermittlung an neues Integral angepasst
    Farbgebung bei der Ergebnisausgabe geändert - Gradient hinzugefügt

[33mcommit 8e37d07958eed87d2f292b1437526dde76f53c73[m[33m ([m[1;31morigin/Integral_bruteForce[m[33m)[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Tue Aug 13 17:34:40 2024 +0200

    Integration als suche im Bereich Flaeche mit Abfrage ob Punkt in Fläche ist implementiert.

[33mcommit 255ff8725bf4a63a0b7154f27f1a596069d5c765[m[33m ([m[1;32mIntegral_Pixelweise[m[33m)[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Tue Aug 13 16:11:46 2024 +0200

    aruIntegral:
     - Um jede Fläche den äußeren Liniezug ergänzt
     - Linie Zeichnen mit OrtAufFlaeche implementiert

[33mcommit 91713493a54924a46abd4d3324dd5e31e06590dd[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun Jul 7 15:47:35 2024 +0200

    Dokumentation "RechnerUnterstütztesZeichnen RUZ Technische Spezifikationen" ergänzt.
    Funktion "Viereck::LageAufFlaeche" an Dokumentaton angepasst
    
    TODO: Zeichnen der Flächen überprüfen!!! aruzeichner scheint nicht richtig zu funktionieren. :(

[33mcommit fe6952c28d570e44523177dc4e7dc5a23a1b3b29[m
Merge: 81ac22c a376cee
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sat May 25 09:28:01 2024 +0200

    Merge branch 'Integral_Pixelweise' of https://github.com/RuebenBaer/ruz into Integral_Pixelweise
    Aufgeräumt

[33mcommit 81ac22cab32563ff9d93abbf326ab43b2b3c0540[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sat May 25 09:23:54 2024 +0200

    unbekannte Änderungen

[33mcommit a376cee54c3e6e14833f12353135956117d1c4e4[m
Merge: fa41b34 a6f1e23
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Thu May 23 23:50:33 2024 +0200

    Merge branch 'Integral_Pixelweise' of https://github.com/RuebenBaer/ruz into Integral_Pixelweise

[33mcommit fa41b3469c986cce3692a56b86a7d3a41df7b043[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Thu May 23 23:47:51 2024 +0200

    Absturz wenn Fenster zu klein ist behoben
    Maßstabsbalken geändert -> Ausgerichtet am unteren Rand des ClientDC

[33mcommit a6f1e2301b0f0ec6079afec9f94bc383eeb60ebf[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Thu May 23 18:28:18 2024 +0200

    Latex weitergeführt

[33mcommit 83340b41f7225be033de7f0b336b5e5027e3ac1f[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun Apr 28 12:42:47 2024 +0200

    Kleiner Änderungen in Latex

[33mcommit 63d1048db65878f81052e5d9f277b6416cad9efa[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun Apr 28 11:40:10 2024 +0200

    Kleiner Änderung in Latex/Grundgleichungen

[33mcommit d1e42401b561d2d62da10f62a7c58751652a6ed2[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Mon Apr 1 12:30:21 2024 +0200

    Latex Update

[33mcommit 6d1224337d87c26bc450f3f1870887ad0310eb1a[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Fri Mar 29 15:20:38 2024 +0100

    Fehler bei Volumenermittlung über Prismen behoben
            (Höhe A > Höhe B --> Höhe A >= Höhe B)
    Latexdokumentation der Berechnungsgrundlagen hinzugefügt (Arbeitsstand 29.03.2024)

[33mcommit 3cd64f854308b76cdcb5ca8387a044fb164edd6d[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sat Feb 3 14:21:56 2024 +0100

    matrix und Kamera auf neuem Stand

[33mcommit 63ff96425bdcca52ddb20c1d55977c55ac3f246c[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sun Dec 31 10:30:40 2023 +0100

    .gitignore aktualisiert: '*.ini' ausgeschlossen

[33mcommit 70ff88bb623b4fe455f73df797b52a43bdde4a68[m
Author: Ansgar Rütten <ansgar.ruetten@gmx.de>
Date:   Tue Dec 19 13:23:58 2023 +0100

    Integration: Trapez wird vor dem Zeichnen rungherum um ein Pixel nach außen gesetzt;
    Trotzdem noch Lücken bei Vierecken

[33mcommit d5f3058728fdc328d6c456154a3564b703941ba3[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun Dec 17 13:09:36 2023 +0100

    OrtAufFlaeche um Abfrage ergänzt (nachfrage ob innerhalb liegt)

[33mcommit 4dd598eeb040e76ed53c69d167e6bcf1260270ec[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sun Dec 10 01:02:02 2023 +0100

    Integral: Schleifenabbruch, wenn Punkt nicht innerhalb Fläche, ergänzt

[33mcommit 12065dbb14c037a56bacdfba7cc16218736b7264[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sat Nov 25 10:07:06 2023 +0100

    Compiler-Warnungen behobenLinie Integrieren vorbereitet

[33mcommit 1de93725c3c289a05e10a87b5d621820395b30cc[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun Nov 19 13:47:04 2023 +0100

    Bug gehoben: Layernamen mit Umlauten und scharfem s
    Compilerwarnungen mit strncpy und strncat größtenteils behoben

[33mcommit 38c383d57d547ec2a9c443d62201483f013d9113[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sat Nov 18 09:55:34 2023 +0100

    Integrationsproblem bei negativen Lagekoordinaten gelöst
    =>Ränder bei Integration noch nicht richtig

[33mcommit c5a126dd79276290e27b39a4e8d355cdcf3c9f4f[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sat Nov 11 10:37:57 2023 +0100

    Integral mit OrtAufFläche

[33mcommit d518089614445e91962db3c5a895bd0a99ad240c[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Fri Nov 3 23:58:05 2023 +0100

    MinMax bei Flächen ergänzt -> zur Beschränkung der Höhe bei OrdinateAufEbene()

[33mcommit 333f3196000970de88e6924a72264816e2d00dd3[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Wed Nov 1 22:49:44 2023 +0100

    "Absturz in aruIntegral::ZeichneTrapezSenkrecht lokalisiert; vorerst mit 'continue' abgewendet => genauere Analyse erforderlich"

[33mcommit 21af0d19e7a23cb6353c8fabeae002b0370f0d10[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sun Oct 29 23:46:51 2023 +0100

    Absturz Flächenintegral weiterverfolgt

[33mcommit 2e7ef12efcf61ff7a1ae42483de740829a330bfd[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun Oct 29 19:36:15 2023 +0100

    Versuch Absturz bei Integral zu beheben;
    Fehler bei "Integriere Fläche" / noch nicht behoben;

[33mcommit d53388d80f95dcfe8503f20d95c36ec388685267[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Fri Oct 27 07:46:04 2023 +0200

    Funktion zur Ermittlung des U-Wertes mit Gefälledämmung hinzugefügt
    toDo: Programminterne Eingabe der WLG und R0 einfügen

[33mcommit d099464bf9c5f1768e2249b55b645c1716f6c73c[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun Oct 22 12:23:34 2023 +0200

    makefile verallgemeinert, bak gelöscht

[33mcommit a55c929b881a8152e1a3023ff572017b472c4ba4[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sun Sep 17 14:28:33 2023 +0200

    .gitignore hinzugefügt

[33mcommit 522d97028936521903ea842c61a25a1aa8e008cb[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sat Sep 16 22:05:00 2023 +0200

    Wechsel der Achsen eingefügt
    
    ToDo: Achsenwechsel in Menü einpflegen

[33mcommit 986b9b9d32db9a9c243d550a783732933810e326[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun Sep 10 13:35:43 2023 +0200

    Flächen mit Auswahlrechteck hinzufügen implementiert ('A'-Taste)

[33mcommit 9ff30921c18d9d608d309deab1cee6d6775a6d78[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Mon May 8 18:31:18 2023 +0200

    dll ergänzt

[33mcommit 5650b5abd17ba6ef0ed53b1bffedb124f993f020[m
Merge: cf038f1 ab495e7
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sun May 7 20:03:33 2023 +0200

    Volumenberechnung: Ausgabe nach Prismenberechnung eingefügt

[33mcommit cf038f15819908002a5e46c61ce8c9721e55b29d[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sun May 7 20:02:09 2023 +0200

    Volumenberechnung: Ausgabe nach Prismenberechnung eingefügt

[33mcommit ab495e7d7295c86495ada695ac7db3e0b31d7e80[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun May 7 19:42:58 2023 +0200

    noch ein Test

[33mcommit 8cafbbf7db6838110a7c8507c5e309a8cfa180d1[m
Merge: 532c289 0df679c
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun May 7 19:38:05 2023 +0200

    Mergetest

[33mcommit 532c289bb4d88f586c0aa5df309051e729fa748d[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun May 7 19:13:46 2023 +0200

    Ergebnisausgabe Volumenberechnung (Prismen)
            - Ausgabe in Datei
            - Dreiecke getrennt nach Layer
            - Dreiecke getrennt nach Auf- / Abtrag
            - Flächeninhalte und Volumina

[33mcommit 841a807967e1efc1dd31ded639baa69bbe5abe34[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Mon May 1 19:00:46 2023 +0200

    Ergebnisausgabe Volumenberechnung (Prismen)
            - Ausgabe in Datei
            - Dreiecke getrennt nach Layer
            - Dreiecke getrennt nach Auf- / Abtrag
            - Flächeninhalte und Volumina
    
    makefile update to wxWidgets-3.2.2.1
    
    Ergebnisausgabe Volumen zwischen Layern ermitteln in Textdatei:
            - Dreiecke auslesen
            - Flächeninhalt der Dreiecke
            - Volumen der Dreiecke
    
    LayerMalen ausgelagert; aktLayer zuletzt malen (Oben liegend)
    
    Ausgabe Ergebnis Massenberechnung vorbereitet+
    
    Ergebnisausgabe Volumenermittlung eingefügt - verbugged
    
    q

[33mcommit 0df679c371227fdccb6c484a0394e490e384aaeb[m
Author: RuebenBaer <ansgar.ruetten@gmx.de>
Date:   Sun May 7 13:46:39 2023 +0200

    Ergebnisausgabe Volumenermittlung eingefügt - verbugged

[33mcommit f632bed5e587e09f858945fc6dccac599493960f[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sat May 6 22:32:46 2023 +0200

    Ausgabe Ergebnis Massenberechnung vorbereitet+

[33mcommit 8ed44897626e433a64e73f22e0187df3b0d116fd[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Fri May 5 07:22:01 2023 +0200

    LayerMalen ausgelagert; aktLayer zuletzt malen (Oben liegend)

[33mcommit 0b5fa361a238f29bd9101f5d58421f86c3d4c7ad[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Mon May 1 19:00:46 2023 +0200

    makefile update to wxWidgets-3.2.2.1

[33mcommit f59f0c110eb91487e950ea025e44623d33315c08[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Sun Mar 12 15:51:12 2023 +0100

    Kleinere Änderungen
            - Maßstabsbalken ergänzt
            - Dialog zur Eingabe der Anzeigeskalierung ergänzt
            - RUZmBIFrame Methoden alphanummerisch sortiert

[33mcommit 4b7482216fcf25cdfed03d24cbd2d5fed1fb3313[m
Author: Ruebenbaer <ansgar.ruetten@gmx.de>
Date:   Tue Jan 3 08:52:01 2023 +0100

    erste Sicherung
