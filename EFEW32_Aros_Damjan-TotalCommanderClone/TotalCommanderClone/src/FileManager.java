import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class FileManager {
    private File currentDirectory;

    public FileManager(String startingPath) {
        this.currentDirectory = new File(startingPath);
    }

    public List<File> listFiles() {
        File[] files = currentDirectory.listFiles();
        List<File> fileList = new ArrayList<>();
        if (files != null) {
            for (File file : files) {
                fileList.add(file);
            }
        }
        return fileList;
    }

    public void changeDirectory(String path) {
        File newDirectory = new File(currentDirectory, path);
        if (newDirectory.isDirectory()) {
            currentDirectory = newDirectory;
        }
    }

    public String getCurrentDirectory() {
        return currentDirectory.getAbsolutePath();
    }
}
