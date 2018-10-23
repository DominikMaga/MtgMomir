package filtermtg;

import java.io.File;
import java.math.BigInteger;
import java.util.Iterator;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import org.json.*;

import org.apache.commons.io.FileUtils;

public class FilterMTG {

    public static void readJSON() throws Exception {
        File file = new File("card.json");
        String content = FileUtils.readFileToString(file, "utf-8");

        // Convert JSON string to JSONObject
        JSONObject jObject = new JSONObject(content);
//    String name =tomJsonObject.getString("name");
//JSONObject passportJsonObject = tomJsonObject.getJSONObject("Aerial Toastmaster");

        jObject = new JSONObject(content.trim());
        Iterator<String> keys = jObject.keys();

        while (keys.hasNext()) {
            String key = keys.next();
            if (jObject.get(key) instanceof JSONObject) {

                JSONObject aJObject = jObject.getJSONObject(key);
                String name = aJObject.getString("name");
                if (name.equals("Little Girl")) {
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

    public static void main(String[] args) throws Exception {
        readJSON();

    }
//    private FileChooser FileChooser;
//    public static void bazaImport(){
//         System.out.println("wczytanie pliku");
//        FileChooser fileChooser=new FileChooser();
//         fileChooser.setTitle("wczytaj baze MTG");
//         FileChooser.ExtensionFilter extFilter= new FileChooser.ExtensionFilter("Json files (*.JSON)","*.JSON");
//         fileChooser.getExtensionFilters().add(extFilter);
//         File file = fileChooser.showOpenDialog(new Stage());
}
//    

//
//
