package virtualcontrol;

import java.awt.*;
import java.awt.event.InputEvent;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.util.LinkedList;

public class App {

    private static App app;
    private static boolean running = true;
    private Robot robot;
    private LinkedList<Integer> pressedKeys;

    public static void main(String[] args) {
        System.out.println("##############################");
        System.out.println("#       VirtualControl       #");
        System.out.println("##############################");

        app = new App();

        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            System.out.println("Shutdown hook started...");

            running = false;

            if (app != null) {
                app.releasePressedKeys();
            }

            System.out.println("Shutdown hook completed...");
        }));

        app.run();
    }

    public void mousePress(int keyCode) {
        if (!pressedKeys.contains(keyCode)) {
            robot.mousePress(keyCode);
            pressedKeys.add(keyCode);
        }
    }

    public void mouseRelease(int keyCode) {
        if (pressedKeys.contains(keyCode)) {
            robot.mouseRelease(keyCode);
            pressedKeys.removeFirstOccurrence(keyCode);
        }
    }

    public void releasePressedKeys() {
        if (pressedKeys == null) {
            return;
        }

        for (Integer pressedKey : pressedKeys) {
            mouseRelease(pressedKey);
        }
    }

    private void run() {
        System.out.println("Virtual control is running!");


        try {
            robot = new Robot();
            pressedKeys = new LinkedList<>();

            BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in));
            while (running) {
                String str = stdin.readLine();
                if (str == null) {
                    break;
                }
                switch (str) {
                    case "p_up": mousePress(InputEvent.BUTTON1_DOWN_MASK); break;
                    case "r_up": mouseRelease(InputEvent.BUTTON1_DOWN_MASK); break;
                    case "p_down": mousePress(InputEvent.BUTTON3_DOWN_MASK); break;
                    case "r_down": mouseRelease(InputEvent.BUTTON3_DOWN_MASK); break;
                    case "g_pos": System.out.println("pos " + MouseInfo.getPointerInfo().getLocation().x + " " + MouseInfo.getPointerInfo().getLocation().y); break;
                }
                if (str.startsWith("m_move")) {
                    String[] split = str.split(" ");
                    if (split.length == 3) {
                        int x = Integer.parseInt(split[1]);
                        int y = Integer.parseInt(split[2]);
                        robot.mouseMove(x, y);
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        System.out.println("Virtual control is stopped!");
    }
}