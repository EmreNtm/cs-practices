import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class Mean {

	public static class CategoryMeanMapper extends Mapper<Object, Text, Text, Text> {
		private final int keyType = 4;
		private Text meanAndCount = new Text();
		private Text word = new Text();
		
		public void map(Object key, Text value, Context context) throws IOException, InterruptedException {		
			String[] tokens = value.toString().split(",");
			int tokenCount = tokens.length;
			
			for (int i = 0; i < tokenCount; i++) {
				if (i == keyType) {
					if (tokens[i].equalsIgnoreCase(""))
						continue;
					word.set(tokens[i]);
				} else if (i == 6) {
					String meanString = tokens[i];
					if (meanString.equalsIgnoreCase(""))
						meanString = "0";
					else if (meanString.equalsIgnoreCase("price"))
						continue;
					meanAndCount.set(meanString + ",1");
					context.write(word, meanAndCount);
				}
			}
		}
		
	}
	
	public static class BrandMeanMapper extends Mapper<Object, Text, Text, Text> {
		private final int keyType = 5;
		private Text meanAndCount = new Text();
		private Text word = new Text();
		
		public void map(Object key, Text value, Context context) throws IOException, InterruptedException {		
			String[] tokens = value.toString().split(",");
			int tokenCount = tokens.length;
			
			for (int i = 0; i < tokenCount; i++) {
				if (i == keyType) {
					if (tokens[i].equalsIgnoreCase(""))
						continue;
					word.set(tokens[i]);
				} else if (i == 6) {
					String meanString = tokens[i];
					if (meanString.equalsIgnoreCase(""))
						meanString = "0";
					else if (meanString.equalsIgnoreCase("price"))
						continue;
					meanAndCount.set(meanString + ",1");
					context.write(word, meanAndCount);
				}
			}
		}
	}
	
	public static class EventMeanMapper extends Mapper<Object, Text, Text, Text> {
		private final int keyType = 1;
		private Text meanAndCount = new Text();
		private Text word = new Text();
		
		public void map(Object key, Text value, Context context) throws IOException, InterruptedException {		
			String[] tokens = value.toString().split(",");
			int tokenCount = tokens.length;
			
			for (int i = 0; i < tokenCount; i++) {
				if (i == keyType) {
					if (tokens[i].equalsIgnoreCase(""))
						continue;
					word.set(tokens[i]);
				} else if (i == 6) {
					String meanString = tokens[i];
					if (meanString.equalsIgnoreCase(""))
						meanString = "0";
					else if (meanString.equalsIgnoreCase("price"))
						continue;
					meanAndCount.set(meanString + ",1");
					context.write(word, meanAndCount);
				}
			}
		}
	}
	
	public static class MeanCombiner extends Reducer<Text, Text, Text, Text> {
		
		public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
			float sum = 0;
			int count = 0;
			for (Text t : values) {
				StringTokenizer itr = new StringTokenizer(t.toString(), ",");
				sum += Float.parseFloat(itr.nextToken());
				count += Integer.parseInt(itr.nextToken());
			}
			context.write(key, new Text((sum / count) + "," + count));
		}
		
	}
	
	public static class MeanReducer extends Reducer<Text, Text, Text, Text> {
		
		public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
			float sum = 0;
			int total = 0;
			float avg = 0;
			float count = 0;
			for (Text t : values) {
				StringTokenizer itr = new StringTokenizer(t.toString(), ",");
				avg = Float.parseFloat(itr.nextToken());
				count = Integer.parseInt(itr.nextToken());
				sum += avg * count;
				total += count;
			}
			context.write(key, new Text(String.format("%.2f",(sum / total))));
		}
		
	}
	
	public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {
		System.out.println("Mean Mode");
		Configuration conf = new Configuration();
		Job job = Job.getInstance(conf, "find mean");
		job.setJarByClass(Mean.class);
		if (Integer.parseInt(args[0]) == 1) {
			//Category
			job.setMapperClass(CategoryMeanMapper.class);
		} else if (Integer.parseInt(args[0]) == 2) {
			//Brand
			job.setMapperClass(BrandMeanMapper.class);
		} else {
			//Event
			job.setMapperClass(EventMeanMapper.class);
		}
		job.setCombinerClass(MeanCombiner.class);
		job.setReducerClass(MeanReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
		Path input = new Path(args[1]);
		Path output = new Path(args[2]);
		FileSystem hdfs = FileSystem.get(conf);
		if (hdfs.exists(output))
		      hdfs.delete(output, true);
		FileInputFormat.addInputPath(job, input);
		FileOutputFormat.setOutputPath(job, new Path(args[2]));
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
	
}

