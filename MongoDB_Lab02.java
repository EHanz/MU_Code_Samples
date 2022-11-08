import java.util.Arrays;

import org.bson.Document;
import org.bson.conversions.Bson;

import com.mongodb.MongoClient;
import com.mongodb.MongoClientURI;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoCursor;
import com.mongodb.client.MongoDatabase;
import com.mongodb.client.model.Accumulators;
import com.mongodb.client.model.Aggregates;
import com.mongodb.client.model.Filters;
import com.mongodb.client.model.Indexes;
import com.mongodb.client.model.Sorts;

public class Airbnb {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		String URI = "mongodb+srv://admin:admin@cluster0.quq6s.mongodb.net/test";
		MongoClientURI mongoClientURI = new MongoClientURI(URI);
		MongoClient mongoClient = new MongoClient(mongoClientURI); 
			
		MongoDatabase database = mongoClient.getDatabase("sample_airbnb");
			
		MongoCollection<Document> collection = database.getCollection("listingsAndReviews");
			
		// Find five airbnbs with more than 50 reviews AND a rating >= 90 AND a price < 150
		Bson query_1 = Filters.and(
			Filters.gt("number_of_reviews", 50),
			Filters.and(
				Filters.gte("review_scores.review_scores_rating", 90),
				Filters.lt("price", 150)
			)
		);
				
		try (MongoCursor<Document> cursor = collection.find(query_1).limit(5).iterator();) {
			
			while (cursor.hasNext()) {
				System.out.println(cursor.next().toJson());
				System.out.println();
			}
			
		}
		System.out.println("================================================================================\n");
		
		// Find five airbnbs with the Apartment property type with a rating > 80 or cleaning scores = 10
		Bson query_2 = Filters.or(
			Filters.and(
				Filters.eq("property_type", "Apartment"),
				Filters.gt("review_scores.review_scores_rating", 90)
			),
			Filters.and(
				Filters.eq("property_type", "Apartment"),
				Filters.eq("review_scores.review_scores_cleaning", 10)
			)
		);
		
		try (MongoCursor<Document> cursor = collection.find(query_2).limit(5).iterator()) {
			
			while (cursor.hasNext()) {
				System.out.println(cursor.next().toJson());
				System.out.println();
			}
			
		}
		System.out.println("================================================================================\n");
		
		// Create an index on "price" in ascending order
		collection.createIndex(Indexes.ascending("price"));
		
		// Create an aggregate pipeline to find the three most popular room types
		Bson stage1 = Aggregates.group("$room_type", Accumulators.sum("totalRating", "$review_scores.review_scores_rating"));
		Bson stage2 = Aggregates.sort(Sorts.descending("totalRating"));
		Bson stage3 = Aggregates.limit(3);
		
		try (MongoCursor<Document> cursor = collection.aggregate(Arrays.asList(stage1, stage2, stage3)).iterator();) {
			
			while (cursor.hasNext()) {
				System.out.println(cursor.next().toJson());
				System.out.println();
			}
			
		}
		System.out.println("================================================================================\n");
		
		// Create an aggregate pipeline to find the top three countries with the lowest average price among Real Bed airbnbs
		Bson stage10 = Aggregates.match(Filters.eq("bed_type", "Real Bed"));
		Bson stage20 = Aggregates.group("$address.country", Accumulators.avg("avgPrice", "$price"));
		Bson stage30 = Aggregates.sort(Sorts.ascending("avgPrice"));
		Bson stage40 = Aggregates.limit(3);
		
		try (MongoCursor<Document> cursor = collection.aggregate(Arrays.asList(stage10, stage20, stage30, stage40)).iterator();) {
			
			while (cursor.hasNext()) {
				System.out.println(cursor.next().toJson());
				System.out.println();
			}
			
		}
			
		mongoClient.close();
	}

}