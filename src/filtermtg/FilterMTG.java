package filtermtg;

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
import javafx.scene.layout.Pane;
import javafx.scene.layout.StackPane;
import javafx.stage.DirectoryChooser;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import oracle.jrockit.jfr.parser.ChunkParser;
import org.json.*;

import org.apache.commons.io.FileUtils;

public class FilterMTG extends Application {

    DirectoryChooser directoryInput;
    DirectoryChooser directoryOutput;
    File outputDirectory;
    File inputDirectory;

    public static void readJSON() throws Exception {
        File file = new File("card.json");
        String content = FileUtils.readFileToString(file, "utf-8");

        // Convert JSON string to JSONObject
        JSONObject jObject = new JSONObject(content);
        jObject = new JSONObject(content.trim());
        Iterator<String> keys = jObject.keys();

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
                if (type.contains("Creature")) {
                    System.out.println(name + " " + cmc);
                }
            }
        }
    }

    //select folder
    @Override
    public void start(Stage primaryStage) {
        
        
        Button btn = new Button();
        btn.setText("Choose input directory");
        Button btn2 = new Button();
        btn2.setText("Choose output directory");
        Button btn3 = new Button();
        btn3.setText("GO!");

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
                    if(directoryInput==null || directoryOutput==null){
                     Alert alert = new Alert(AlertType.ERROR);
                     String err = new String("");
                     if(directoryInput==null){
                         err=err+"+No choosed input directory ";
                     }
                     if(directoryOutput==null){
                         err=err+ "+No choosed output directory";
                     }
                     alert.setTitle("No choosed directory");
                     alert.setHeaderText("");
                     alert.setContentText(err);
                     alert.showAndWait();
                    }
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
        btn3.setLayoutX(250);
        btn3.setLayoutY(200);
        root.getChildren().add(btn);
        root.getChildren().add(btn2);
         root.getChildren().add(btn3);
        Scene scene = new Scene(root, 300, 250);

        primaryStage.setTitle("Mtg Card Momir Selecter");
        primaryStage.setScene(scene);
        primaryStage.show();
    }


    public static void main(String[] args) throws Exception {
  
        launch(args);
    }
}
