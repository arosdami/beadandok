import javax.swing.SwingUtilities;

public class Main {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            FileManagerGUI gui = new FileManagerGUI();
            gui.setVisible(true);
        });
    }
}
