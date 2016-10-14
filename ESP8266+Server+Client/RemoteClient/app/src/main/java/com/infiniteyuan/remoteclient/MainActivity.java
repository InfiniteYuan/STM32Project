package com.infiniteyuan.remoteclient;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.view.View;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.SeekBar;
import android.widget.Toast;

import com.infiniteyuan.Entity.Motor;

public class MainActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    private SeekBar mSeekBar1;
    private SeekBar mSeekBar2;
    private SeekBar mSeekBar3;
    private SeekBar mSeekBar4;
    private SeekBar mSeekBar5;
    private SeekBar mSeekBar6;

    private Motor mMotor;
    private ToServer mToServer;
    private String deviceName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        init();
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.setDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);
    }

    public void init() {
        deviceName = "1:2:3:4:5:a";
        mToServer = new ToServer();
        mMotor = new Motor();

        new Thread(UpdateMotorStatusRunnable).start();//更新Motor状态

        mSeekBar1 = (SeekBar) findViewById(R.id.seekBar1);
        mSeekBar2 = (SeekBar) findViewById(R.id.seekBar2);
        mSeekBar3 = (SeekBar) findViewById(R.id.seekBar3);
        mSeekBar4 = (SeekBar) findViewById(R.id.seekBar4);
        mSeekBar5 = (SeekBar) findViewById(R.id.seekBar5);
        mSeekBar6 = (SeekBar) findViewById(R.id.seekBar6);

        mSeekBar1.setOnSeekBarChangeListener(new MySeekBarChangeListener(mMotor, 1, mToServer, getApplicationContext()));
        mSeekBar2.setOnSeekBarChangeListener(new MySeekBarChangeListener(mMotor, 2, mToServer, getApplicationContext()));
        mSeekBar3.setOnSeekBarChangeListener(new MySeekBarChangeListener(mMotor, 3, mToServer, getApplicationContext()));
        mSeekBar4.setOnSeekBarChangeListener(new MySeekBarChangeListener(mMotor, 4, mToServer, getApplicationContext()));
        mSeekBar5.setOnSeekBarChangeListener(new MySeekBarChangeListener(mMotor, 5, mToServer, getApplicationContext()));
        mSeekBar6.setOnSeekBarChangeListener(new MySeekBarChangeListener(mMotor, 6, mToServer, getApplicationContext()));
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();

        if (id == R.id.nav_camera) {
            // Handle the camera action
        } else if (id == R.id.nav_gallery) {

        } else if (id == R.id.nav_slideshow) {

        } else if (id == R.id.nav_manage) {

        } else if (id == R.id.nav_share) {

        } else if (id == R.id.nav_send) {

        }

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    Runnable UpdateMotorStatusRunnable = new Runnable() {
        @Override
        public void run() {
            Message message = new Message();
            Bundle bundle = new Bundle();
            if (!mToServer.QueryMotor(deviceName, mMotor)) {
                bundle.putBoolean("result", false);
            } else {
                bundle.putBoolean("result", true);
            }
            UpdateMotorStatusHandler.sendMessage(message);
        }
    };

    Handler UpdateMotorStatusHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            Bundle bundle = msg.getData();
            if (bundle.getBoolean("result", false)) {
                mSeekBar1.setProgress(mMotor.Motor[0]);
                mSeekBar2.setProgress(mMotor.Motor[1]);
                mSeekBar3.setProgress(mMotor.Motor[2]);
                mSeekBar4.setProgress(mMotor.Motor[3]);
                mSeekBar5.setProgress(mMotor.Motor[4]);
                mSeekBar6.setProgress(mMotor.Motor[5]);

                Toast.makeText(getApplicationContext(), "更新完成!", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(getApplicationContext(), "更新失败!", Toast.LENGTH_SHORT).show();
            }
        }
    };
}
