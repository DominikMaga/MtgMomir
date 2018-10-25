package filtermtg;

import com.sun.media.sound.ModelStandardTransform;
import java.awt.image.BufferedImage;
import java.awt.image.WritableRaster;
import java.io.File;
import java.math.BigInteger;
import java.util.Iterator;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;
import javafx.scene.layout.StackPane;
import javafx.stage.DirectoryChooser;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import javax.imageio.ImageIO;
import javax.xml.soap.Text;
import marvin.MarvinPluginCollection;
import static marvin.MarvinPluginCollection.scale;
import static marvin.MarvinPluginCollection.thresholding;
import marvin.image.MarvinImage;
import marvin.io.MarvinImageIO;
import oracle.jrockit.jfr.parser.ChunkParser;
import org.apache.commons.io.FileUtils;
import org.json.*;
import sun.awt.image.ImagingLib;

public class FilterMTG extends Application {

    DirectoryChooser directoryInput;
    DirectoryChooser directoryOutput;
    static File outputDirectory;
    static File inputDirectory;
    static File CMCFolder;
    static String namePack = "";
//    File image;

    public static void readJSON() throws Exception {
        File file = new File("card.json");
        String content = FileUtils.readFileToString(file, "utf-8");

        // Convert JSON string to JSONObject
        JSONObject jObject = new JSONObject(content);
        jObject = new JSONObject(content.trim());
        Iterator<String> keys = jObject.keys();
        File[] listOfFilesInput = inputDirectory.listFiles();
        File[] listOfFilesOutput = outputDirectory.listFiles();

        while (keys.hasNext()) {
            String key = keys.next();
            if (jObject.get(key) instanceof JSONObject) {

                JSONObject aJObject = jObject.getJSONObject(key);
                String name = aJObject.getString("name");
                if (name.equals("Little Girl")) { // cmc = 0.5
                    continue;
                }
                String type = aJObject.getString("type");
                BigInteger cmc = aJObject.getBigInteger("cmc");
                String enlargment = new String(".full.jpg");
                if (type.contains("Creature")) {
                    if (listOfFilesOutput.length == 0) {
                        if (listOfFilesInput.length != 0) {

                            for (File lif : listOfFilesInput) {

                                if (lif.getName().contentEquals(name + enlargment)) {
                                    CMCFolder = new File(outputDirectory + "\\" + namePack + cmc.toString());
                                    if (!CMCFolder.exists()) {
                                        CMCFolder.mkdirs();
                                    }

                                    lif.renameTo(new File(CMCFolder + "\\" + name + ".full.jpg"));
                                    System.out.println(name + " !!!!");
//                                    FileUtils.copyFile(source, dest);
                                }
                            }
                            listOfFilesOutput = outputDirectory.listFiles();
                        }
                    } else {
                        if (listOfFilesInput.length != 0) {
                            for (File lif : listOfFilesInput) {
                                for (File lof : listOfFilesOutput) {
                                    if (lof.getName().contains(lif.getName())) {
                                        continue;
                                    } else {
                                        if (lif.getName().contentEquals(name + enlargment)) {
                                            CMCFolder = new File(outputDirectory + "\\" + namePack + cmc.toString());
                                            if (!CMCFolder.exists()) {
                                                CMCFolder.mkdirs();
                                            }
                                            MarvinImage original = MarvinImageIO.loadImage(lif.toString());
                                            MarvinImage output = original.clone();
                                            scale(original, output, 384);
                                            thresholding( output, output,150);
                                            
                                            MarvinImageIO.saveImage(output, CMCFolder + "/" + name + ".full.png");
//                                            
//                                    lif.renameTo(new File(CMCFolder + "\\" + name + ".full.jpg"));
//                                    System.out.println(name);
                                        }
                                    }
                                }
                            }

                        }

                    }
                }
            }
        }
        
        
                        Alert alert = new Alert(AlertType.INFORMATION);

                        alert.setTitle("Succes");
                        alert.setHeaderText("");
                        alert.setContentText("Done!");
                        alert.showAndWait();
        
        
        
    }
//    public static BufferedImage thresholdImage(BufferedImage image, int threshold) {
//    BufferedImage result = new BufferedImage(image.getWidth(), image.getHeight(), BufferedImage.TYPE_BYTE_GRAY);
//    result.getGraphics().drawImage(image, 0, 0, null);
//    WritableRaster raster = result.getRaster();
//    int[] pixels = new int[image.getWidth()];
//    for (int y = 0; y < image.getHeight(); y++) {
//        raster.getPixels(0, y, image.getWidth(), 1, pixels);
//        for (int i = 0; i < pixels.length; i++) {
//            if (pixels[i] < threshold) pixels[i] = 0;
//            else pixels[i] = 255;
//        }
//        raster.setPixels(0, y, image.getWidth(), 1, pixels);
//    }
//    return result;
//}

    //select folder
    @Override
    public void start(Stage primaryStage) {

        Button btn = new Button();
        btn.setText("Choose input directory");
        Button btn2 = new Button();
        btn2.setText("Choose output directory");
        Button btn3 = new Button();
        btn3.setText("GO!");
        TextField txtf = new TextField();
        txtf.setText("");

        btn.setOnAction(new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent event) {
                directoryInput = new DirectoryChooser();
                inputDirectory = directoryInput.showDialog(primaryStage);

                if (inputDirectory == null) {
                    //No Directory selected
                } else {
                    System.out.println(inputDirectory.getAbsolutePath());
                }
            }
        });

        btn2.setOnAction(new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent event) {
                directoryOutput = new DirectoryChooser();
                outputDirectory = directoryOutput.showDialog(primaryStage);

                if (outputDirectory == null) {
                    //No Directory selected
                } else {
                    System.out.println(outputDirectory.getAbsolutePath());
                }
            }
        });
        btn3.setOnAction(new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent event) {
                try {
                    if (directoryInput == null || directoryOutput == null) {
                        Alert alert = new Alert(AlertType.ERROR);
                        String err = new String("");
                        if (directoryInput == null) {
                            err = err + "+No choosed input directory ";
                        }
                        if (directoryOutput == null) {
                            err = err + "+No choosed output directory";
                        }
                        alert.setTitle("No choosed directory");
                        alert.setHeaderText("");
                        alert.setContentText(err);
                        alert.showAndWait();
                    }
                    namePack = txtf.getText();
                    readJSON();
                } catch (Exception ex) {
                    Logger.getLogger(FilterMTG.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        });

        Pane root = new Pane();

        btn.setLayoutX(10);
        btn.setLayoutY(20);
        btn2.setLayoutX(10);
        btn2.setLayoutY(50);
        btn3.setLayoutX(450);
        btn3.setLayoutY(365);
        txtf.setLayoutX(320);
        txtf.setLayoutY(20);

        root.getChildren().add(btn);
        root.getChildren().add(btn2);
        root.getChildren().add(btn3);
        root.getChildren().add(txtf);
        Scene scene = new Scene(root, 500, 400);

        primaryStage.setTitle("Mtg Card Momir Selecter");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    public static void main(String[] args) throws Exception {

        launch(args);
    }
}
