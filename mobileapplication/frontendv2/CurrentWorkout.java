package com.example.finaldemo;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.graphics.ColorMatrix;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Chronometer;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Set;
import java.util.UUID;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class CurrentWorkout extends AppCompatActivity {
    private BluetoothAdapter bAdapter;
    private BluetoothDevice mDevice;
    private ConnectThread mConnectThread;
    private ConnectedThread mConnectedThread;
    private Handler mHandler;

    public static final String EXTRA_NUMBER = "com.example.testappver1.EXTRA_NUMBER";
    //private String BASE_URL = "http://192.168.1.27:3000";
    private String BASE_URL = "http://192.168.254.17:3000";
    Retrofit retroFit = new Retrofit.Builder()
            .baseUrl(BASE_URL) // local host
            .addConverterFactory(GsonConverterFactory.create())
            .build();

    RetrofitInterface retrofitInterface = retroFit.create(RetrofitInterface.class);
    private Chronometer chronometer;
    private boolean running;
    private long pauseOffset, elapsedTime;          // needed to calculate time diff. chronometer runs in background

    private Button button;                          // finish workout button
    // instantiate the matrix
    ArrayList<View> SquareListR = new ArrayList<>();
    ArrayList<View> SquareListL = new ArrayList<>();
    private int RightCounter = 0;
    private int LeftCounter = 0;

    ArrayList<Integer> integerArrayListLeft = new ArrayList<>();
    ArrayList<Integer> integerArrayListRight = new ArrayList<>();

    int stepCount = 0;
    // used for workout number of elapsed time
    int number;
    // test button
    private Button testBtn, testBtn1;
    private TextView stepView;
    private TextView editDistance, editSteps;
    int userId, userHeight;
    double strideLength, distanceTraveled = 0.0, feet;
    String userName;
    String stringLeft;
    String stringRight;
    String stringy = "";
    char side;
    boolean seen255 = true;
    private int dataReceived = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_current_workout);
        //userId = getIntent().getIntExtra("USER", 0);
        userName = getIntent().getStringExtra("USER_NAME");
        userHeight = getIntent().getIntExtra("USER_HEIGHT", 0);
        userId = getIntent().getIntExtra("USER_ID", 0);
        editDistance = findViewById(R.id.editDistance);
        //Toast.makeText(this, "STRING NAME: " + userName, Toast.LENGTH_LONG).show();
        //Intent intent = new Intent(this, FinishWorkout.class);

        strideLength = userHeight * 0.43; // height multiplied by this for stride length
        feet = strideLength / 12;
        // right sole
        for (int i = -1; i < 255; i++) {
            String squareID = "t" + (i + 1);
            int resID = getResources().getIdentifier(squareID, "id", getPackageName());
            SquareListR.add(((View) findViewById(resID)));   // add all View type to ArrayList
        }
        // left sole
        for (int i = -1; i < 255; i++) {
            String squareID = "s" + (i + 1);
            int resID = getResources().getIdentifier(squareID, "id", getPackageName());
            SquareListL.add(((View) findViewById(resID)));   // add all View type to ArrayList
        }
        chronometer = findViewById(R.id.chronometer);
        chronometer.setOnChronometerTickListener(new Chronometer.OnChronometerTickListener() {
            @Override
            public void onChronometerTick(Chronometer chronometer) {
                long time = SystemClock.elapsedRealtime() - chronometer.getBase();
                int h = (int)(time/3600000);
                int m = (int)(time - h*3600000)/60000;
                int s = (int)(time - h*3600000-m*60000)/1000;
                String hh = h < 10 ? "0"+h: h+"";
                String mm = m < 10 ? "0"+m: m+"";
                String ss = s < 10 ? "0"+s: s+"";
                chronometer.setText(hh+":"+mm+":"+ss);
            }
        });
        chronometer.setBase(SystemClock.elapsedRealtime());
        chronometer.start();        // start time upon opening
        running = true;
       // stepView = findViewById(R.id.editSteps);
        button = findViewById(R.id.finish);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {           // open new activity (finishedWorkout) after btn pressed
                mConnectedThread.write(String.valueOf(0xFF));
                mConnectedThread.cancel();
                mConnectThread.cancel();
                openFinishWorkout();
            }
        });
        number = getIntent().getIntExtra("WORKOUT_NUMBER", 0);
        editSteps = findViewById(R.id.editSteps);
        // handler
        mHandler = new Handler(new Handler.Callback() {
            int pos = 0;
            @Override
            public boolean handleMessage(android.os.Message msg) {
                //Toast.makeText(getApplicationContext(), "Message Received", Toast.LENGTH_SHORT).show();
                int intReceived = (int)msg.obj;
                switch (msg.what) {
                    case 1:
                        // is the next value a step boolean or a side value
                        if (seen255){
                            if(intReceived == 1)
                                stepCount++;
                            seen255 = false;
                        }
                        else if(intReceived == 253)
                            side = 'L';
                        else if (intReceived == 254)
                            side = 'R';
                        // call matrix for side
                        else if (side == 'L')
                            colorMatrixLeft(intReceived);
                        else if (side == 'R')
                            colorMatrixRight(intReceived);

                        //Log.i("RECEIVED: ", intReceived + " : " + String.valueOf(pos));
                        //colorMatrixLeft(intReceived);
                        //display.setText(String.valueOf(intReceived));
                        Log.i("received", String.valueOf(intReceived));
                        break;
                }
                return true;
            }
        });

        // Bluetooth stuff
        // enable the adapter
        bAdapter = BluetoothAdapter.getDefaultAdapter();
        // check if BT is supported
        if(bAdapter == null) {
            Toast.makeText(getApplicationContext(), "Bluetooth is NOT supported", Toast.LENGTH_SHORT).show();
        }
        // check if BT is enabled, if not then request it be turned on
        if(!bAdapter.isEnabled()) {
            Intent enableBT = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
        }

        Set<BluetoothDevice> pairedDevices = bAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            for(BluetoothDevice device : pairedDevices) {
                mDevice = device;
            }
        }
        mConnectThread = new ConnectThread(mDevice);
        mConnectThread.start();
        }

    private class ConnectThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;
        private static final String TAG = "ConnectThread";

        private final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");

        public ConnectThread(BluetoothDevice device) {
            BluetoothSocket tmp = null;
            mmDevice = device;
            try {
                tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
            } catch (IOException e) {
                Log.e(TAG, "COouldnot create RFCOMM socket:" + e.toString(), e);
            }
            mmSocket = tmp;
        }

        public void run(){
            // cancel discovery because it slows down connection speed
            bAdapter.cancelDiscovery();
            try {
                // connect to device through the socket
                mmSocket.connect();
            } catch (IOException connectException) {
                // unable to connect; close socket and return
                try {
                    mmSocket.close();
                } catch(IOException closeException) {
                    Log.e(TAG, "Could not close connection:" + closeException);
                }
                return;
            }

            mConnectedThread = new ConnectedThread(mmSocket);
            mConnectedThread.start();
            String sendMsg = userName + " " +  String.valueOf(userId);
            mConnectedThread.write(sendMsg);
            Log.e("sent", String.valueOf(sendMsg));
        }

        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e){
                Log.e(TAG, "Could not close conenction:" + e);
            }
        }
    }

    private class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream inStream;
        private final OutputStream outStream;
        private ArrayList list = new ArrayList();
        private static final String TAG = "ConnectedThread";


        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            try {
                tmpIn = socket.getInputStream();
            } catch (IOException e) {
                Log.e(TAG, "Error creating input stream", e);
            }
            try {
                tmpOut = socket.getOutputStream();
            } catch (IOException e) {
                Log.e(TAG, "Error creating output stream", e);
            }

            inStream = tmpIn;
            outStream = tmpOut;
        }

        public void run() {
            int numBytes; // number of bytes returned from read
            int i =0;
            int[] intReceived = new int[124];
            while (true) {
                try {
                    numBytes = inStream.read();
                    //Log.i("STORING", String.valueOf(intReceived));
                    mHandler.obtainMessage(1, numBytes).sendToTarget();
                    //intReceived[i] = numBytes;
                    // if(numBytes == 255){
                    //mHandler.obtainMessage(1, intReceived).sendToTarget();
                    //Log.i("STORING", Arrays.toString(intReceived));
                    //mHandler.obtainMessage(1, intReceived);
                    //  i = 0;
                    // }
                   /* if(inStream.available() > 0) {
                        numBytes += inStream.read(buf, numBytes, buf.length - numBytes);
                        for(int i = begin; i < numBytes; i++) {
                            if(buf[i] == "\n".getBytes()[0]) {
                                mHandler.obtainMessage(1, begin, i, buf).sendToTarget();
                                begin = i + 1;
                                if(i == numBytes - 1) {
                                    numBytes = 0;
                                    begin = 0;
                                }
                            }
                        }

                    }*/
                } catch (IOException e) {
                }
            }
        }
        public void write(String input) {
            byte[] bytes = input.getBytes(); //converts entered String into bytes
            try {
                outStream.write(bytes);
                outStream.write(13);
            } catch (IOException e) {
                Log.e("Send Error","Unable to send message",e);
            }
        }

        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e){
                Log.e(TAG, "Could not close connection:" + e);
            }
        }
    }

    private void colorMatrixRight(int testStep){
        //editCalories.setText("" + RightCounter);
        if (testStep == 255){
            distanceTraveled += feet;
            editDistance.setText(String.format("%.2f ", distanceTraveled));
            //editDistance.setText(""+distanceTraveled + "ft");
            String string;
            if (stringy.length() == 768)
                stringy = "";
            for (int i = 0; i < integerArrayListRight.size(); i++) {
                string = integerArrayListRight.get(i).toString();
                stringify(string);
            }
            editSteps.setText("" + stepCount);
            elapsedTime = SystemClock.elapsedRealtime() - chronometer.getBase();
            String mintim = String.valueOf(elapsedTime/1000);
            Step step = new Step('R', stringy, mintim, stepCount, dataReceived);
            Call<Step> call = retrofitInterface.createStep(number, step);
            call.enqueue(new Callback<Step>() {
                @Override
                public void onResponse(Call<Step> call, Response<Step> response) {
                    if (!response.isSuccessful()) {
                        Toast.makeText(getApplicationContext(), "Not Successful", Toast.LENGTH_SHORT).show();
                        return;
                    }
                    Step responseStep = response.body();
                }
                @Override
                public void onFailure(Call<Step> call, Throwable t) {
                    Toast.makeText(getApplicationContext(), "Failed. " + t.getCause(),Toast.LENGTH_SHORT).show();
                }
            });
            dataReceived++;
            integerArrayListRight.clear();
            seen255 = true;
            RightCounter = 0;
        } else {
            if (testStep <= 0)
                SquareListR.get(RightCounter).setBackgroundColor(0xFFFFFFFF);
            else if (testStep < 65)
                SquareListR.get(RightCounter).setBackgroundColor(0xFFD65F59);
            else if (testStep < 130)
                SquareListR.get(RightCounter).setBackgroundColor(0xFFBD1628);
            else if (testStep < 190)
                SquareListR.get(RightCounter).setBackgroundColor(0xFF8D0101);
            else if (testStep < 230)
                SquareListR.get(RightCounter).setBackgroundColor(0xFF680101);
            else
                SquareListR.get(RightCounter).setBackgroundColor(0xFF420C09);
            RightCounter++;
            integerArrayListRight.add(testStep);
        }
    }
    private void colorMatrixLeft(int testStep){
        //editCalories.setText("" + counter);
        if (testStep == 255){
            distanceTraveled += feet;
            editDistance.setText(String.format("%.2f ", distanceTraveled));
            String string;
            if (stringy.length() == 768)
                stringy = "";
            for (int i = 0; i < integerArrayListLeft.size(); i++) {
                string = integerArrayListLeft.get(i).toString();
                stringify(string);
            }
            editSteps.setText("" + stepCount);
            elapsedTime = SystemClock.elapsedRealtime() - chronometer.getBase();
            String mintim = setTime();
            Step step = new Step('L', stringy, mintim, stepCount, dataReceived);
            Call<Step> call = retrofitInterface.createStep(number, step);
            call.enqueue(new Callback<Step>() {
                @Override
                public void onResponse(Call<Step> call, Response<Step> response) {
                    if (!response.isSuccessful()) {
                        Toast.makeText(getApplicationContext(), "Not Successful", Toast.LENGTH_SHORT).show();
                        return;
                    }
                    Step responseStep = response.body();
                }
                @Override
                public void onFailure(Call<Step> call, Throwable t) {
                    Toast.makeText(getApplicationContext(), "Failed. " + t.getCause(),Toast.LENGTH_SHORT).show();
                }
            });
            dataReceived++;
            integerArrayListLeft.clear();
            seen255 = true;
            LeftCounter = 0;
        }
        else {
            if (testStep <= 0)
                SquareListL.get(LeftCounter).setBackgroundColor(0xFFFFFFFF);
            else if (testStep < 65)
                SquareListL.get(LeftCounter).setBackgroundColor(0xFFD65F59);
            else if (testStep < 130)
                SquareListL.get(LeftCounter).setBackgroundColor(0xFFBD1628);
            else if (testStep < 190)
                SquareListL.get(LeftCounter).setBackgroundColor(0xFF8D0101);
            else if (testStep < 230)
                SquareListL.get(LeftCounter).setBackgroundColor(0xFF680101);
            else
                SquareListL.get(LeftCounter).setBackgroundColor(0xFF420C09);
            LeftCounter++;
            integerArrayListLeft.add(testStep);
        }
    }
    private void stringify(String num){
        if (num.length() == 1)
            stringy += "00" + num;
        else if (num.length() == 2)
            stringy += "0" + num;
        else
            stringy += num;

    }
    public void openFinishWorkout(){
        chronometer.stop();
        String mins = setTime();
        //updateWorkout(pauseOffset);
        putElapsed(pauseOffset);
        Intent intent = new Intent(this, FinishWorkout.class);
        Bundle extras = new Bundle();
        extras.putInt("WORKOUT_ID", number);
        extras.putInt("TOTAL_STEPS", stepCount);
        extras.putInt("STEP_COUNT", dataReceived);
        extras.putString("ELAPSED_TIME", mins);
        extras.putString("USER_NAME", userName);
        extras.putInt("USER_ID", userId);
        extras.putInt("USER_HEIGHT", userHeight);
        String sDistance = String.format("%.2f", distanceTraveled);
        extras.putString("DISTANCE", sDistance);
        intent.putExtras(extras);
        startActivity(intent);
    }
    private void putElapsed(long pauseOffset) {
        String mins = setTime();
        // send mins and stepCount to database
        Workout workout = new Workout(number, mins, stepCount);
       // Toast.makeText(getApplicationContext(), "WORKOUTID: " + number, Toast.LENGTH_SHORT).show();
        Call<Workout> call = retrofitInterface.putElapsed(number, workout);
        call.enqueue(new Callback<Workout>() {
            @Override
            public void onResponse(Call<Workout> call, Response<Workout> response) {
                if (!response.isSuccessful()) {
                    Toast.makeText(getApplicationContext(), "Not Successful", Toast.LENGTH_SHORT).show();
                    return;
                }
                Workout postWorkout = response.body();
            }
            @Override
            public void onFailure(Call<Workout> call, Throwable t) {
                Toast.makeText(getApplicationContext(), "Failed. " + t.getCause(),Toast.LENGTH_SHORT).show();
            }
        });
    }
    public void onToggleClicked(View v) {
        boolean on = ((ToggleButton) v).isChecked(); // check toggled button
        if (!on) {    // resume chronometer
            chronometer.setBase(SystemClock.elapsedRealtime() - pauseOffset); // when to start counting, when start btn pressed
            chronometer.start();
            running = true;
        } else {      // pause chronometer
            chronometer.stop();     // only stops the text, not the chronometer from counting
            pauseOffset = SystemClock.elapsedRealtime() - chronometer.getBase(); // used to calculate time
            running = false;
        }
    }
    public String setTime(){
        long timer = SystemClock.elapsedRealtime() - chronometer.getBase();
        int h = (int)(timer/3600000);
        int m = (int)(timer - h*3600000)/60000;
        int s = (int)(timer - h*3600000-m*60000)/1000;
        String hh = h < 10 ? "0"+h: h+"";
        String mm = m < 10 ? "0"+m: m+"";
        String ss = s < 10 ? "0"+s: s+"";
        return (hh+":"+mm+":"+ss);
    }
}
