package c19.hulcy.glprimitivesdemo;

import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

public class GLPrimitivesDemoActivity extends ListActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		String[] glPrimitives = getResources().getStringArray( R.array.gl_primitives );
		ArrayAdapter<String> glPrimitivesAdapter = new ArrayAdapter<String>(this, R.layout.list_item, glPrimitives );
		
		setListAdapter( glPrimitivesAdapter );
		
		ListView lv = getListView();
	
		lv.setOnItemClickListener(new OnItemClickListener()
	      {
	        public void onItemClick( AdapterView<?> parent, View view, int position, long id )
	        {
	        	Intent descIntent = new Intent( getApplicationContext(), GLPrimitiveDescActivity.class );
	        	
	        	descIntent.putExtra( "primitive_selected", ( (TextView) view ).getText() );
	        	
	        	startActivity( descIntent );
	        	
	          
	        }
	      });
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.glprimitives_demo, menu);
		return true;
	}

}
