package org.example;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        ArrayList<Katona> katonak = new ArrayList<>();
        File fajl = new File("katonak.txt");

        try {
            Scanner olvaso = new Scanner(fajl);

            while (olvaso.hasNextLine()) {
                String sor = olvaso.nextLine();
                String[] adatok = sor.split(";");
                // Tisztítjuk a második elemet (fizetés) a felesleges szóközöktől
                Katona tmp = new Katona(adatok[0].trim(), Integer.parseInt(adatok[1].trim()));
                katonak.add(tmp);
            }

            olvaso.close();
        } catch (FileNotFoundException ex) {
            System.out.println("A katonak.txt fájl nem található");
            System.exit(100);
        } catch (NumberFormatException ex) {
            System.out.println("Hiba történt a fizetés beolvasásakor: " + ex.getMessage());
            System.exit(101);
        }

        int maxFizetes = 0;
        String maxNev = "";

        for (Katona katona : katonak) {
            if (katona.getFizetes() > maxFizetes) {
                maxFizetes = katona.getFizetes();
                maxNev = katona.getNev();
            }
        }

        if (!maxNev.isEmpty()) {
            System.out.printf("A legnagyobb fizetéssel rendelkező katona: %s\nFizetése: %sFt\n",
                    maxNev, String.format("%,d", maxFizetes));
        } else {
            System.out.println("Nincsenek katonák az adatbázisban.");
        }
    }
}
