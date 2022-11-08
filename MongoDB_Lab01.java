import com.mongodb.MongoClient;
import com.mongodb.MongoClientURI;
import com.mongodb.client.MongoDatabase;
import com.mongodb.client.MongoCollection;
import org.bson.Document;
import org.bson.conversions.Bson;
import com.mongodb.client.result.DeleteResult;
import static com.mongodb.client.model.Filters.eq;
import com.mongodb.BasicDBObject;
import com.mongodb.client.MongoCursor;


public class MongodbBasics {

	public static void main(String[] args) {


		String uri = "mongodb+srv://admin:admin@cluster0.quq6s.mongodb.net/test";
		MongoClientURI clientURI = new MongoClientURI(uri);
		MongoClient client = new MongoClient(clientURI);
		MongoDatabase database = client.getDatabase("sample_restaurants");

		MongoCollection<Document> coll = database.getCollection("characters");

		Document docu = new Document("name", "John Wick");
		docu.append("sex", "Male");
		docu.append("age", "35");
		docu.append("specialty", "Gun Fight");
		coll.insertOne(docu);

		Document docu2 = new Document ("name", "Hermione Granger");
		docu2.append("sex", "female");
		docu2.append("age", "41");
		docu2.append("specialty", "magical spell");
		coll.insertOne(docu2);
		
		
		Bson deleteKey = eq("name", "John Wick");
		DeleteResult result = coll.deleteOne(deleteKey);

		MongoCollection<Document> collection = database.getCollection("restaurants");
		Document doc = new Document("cuisine", "Chinese");
		doc.append("borough", "Brooklyn");
		Document found = (Document) collection.find(doc).first();
		if (found != null) {
			System.out.println(found.toJson());
		}
		System.out.println("============================================================");

		
		BasicDBObject regexQuery = new BasicDBObject();
		regexQuery.put("name", new BasicDBObject("$regex", "^K.*"));
		regexQuery.append("cuisine", "American");
		try (MongoCursor<Document> cursor = collection.find(regexQuery).iterator();){
			while (cursor.hasNext()) {
				System.out.println(cursor.next().toJson());
			}
		} 
		System.out.println("============================================================");

		
		Document doc2 = new Document("cuisine", "Chinese");
		Document found2 = (Document) collection.find(doc2).first();
		System.out.println("***Before Update******" + found2.toJson());
		Bson updateValue = new Document("name", "Shun Lee No.1");
		Bson update = new Document("$set", updateValue);
		collection.updateOne(found2, update);
		System.out.println("***After Update******" + found2.toJson());
		System.out.println("============================================================");
		

		client.close();
	}

}