import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.File;  // Hozzáadott import
import java.util.List;

public class FileManagerGUI extends JFrame {
    private FileManager fileManager;
    private JList<File> fileList;
    private DefaultListModel<File> listModel;

    public FileManagerGUI() {
        fileManager = new FileManager(System.getProperty("user.home"));
        setTitle("Total Commander Clone");
        setSize(600, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // Stílusos dizájn
        JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        listModel = new DefaultListModel<>();
        fileList = new JList<>(listModel);
        fileList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        fileList.setFont(new Font("Arial", Font.PLAIN, 14));
        fileList.setBackground(Color.LIGHT_GRAY);
        fileList.setForeground(Color.BLACK);
        fileList.setFixedCellHeight(30);
        fileList.setFixedCellWidth(200);
        JScrollPane scrollPane = new JScrollPane(fileList);
        panel.add(scrollPane, BorderLayout.CENTER);

        JButton refreshButton = new JButton("Refresh");
        refreshButton.addActionListener(new RefreshAction());
        panel.add(refreshButton, BorderLayout.SOUTH);

        add(panel, BorderLayout.CENTER);
        refreshFileList();

        // Nyíl billentyűk eseménykezelője
        fileList.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_UP) {
                    int selectedIndex = fileList.getSelectedIndex();
                    if (selectedIndex > 0) {
                        fileList.setSelectedIndex(selectedIndex - 1);
                    }
                } else if (e.getKeyCode() == KeyEvent.VK_DOWN) {
                    int selectedIndex = fileList.getSelectedIndex();
                    if (selectedIndex < listModel.getSize() - 1) {
                        fileList.setSelectedIndex(selectedIndex + 1);
                    }
                }
            }
        });
    }

    private void refreshFileList() {
        listModel.clear();
        List<File> files = fileManager.listFiles();
        for (File file : files) {
            listModel.addElement(file);
        }
        // Az első fájl kiválasztása
        if (!listModel.isEmpty()) {
            fileList.setSelectedIndex(0);
        }
    }

    private class RefreshAction implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent e) {
            refreshFileList();
        }
    }
}
