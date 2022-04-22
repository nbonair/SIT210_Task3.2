// This #include statement was automatically added by the Particle IDE.
#include <BH1750.h>

#define MONITOR_DELAY  2000

BH1750 sensor(0x23, Wire);
bool sun;
bool calibrated;
int luminance;
int lum_sum, monitor_times;
float lum_avr;
int sunlight = 15000;

void setup()
{
    sensor.begin();
    sensor.set_sensor_mode(BH1750::forced_mode_high_res2);
    sun = false;
    calibrated = false;
}

void calibrate_sun()
{
    calibrated = true;
    for (int i =0; i<=300;i++)
    {
        luminance = sensor.get_light_level();
        if (sunlight >= luminance)
        {
            sunlight = luminance;
        }
    }
}
void loop()
{
    sensor.make_forced_measurement();
    
    if (!calibrated)
    {
        calibrate_sun();
    }
    else
    {
        luminance = sensor.get_light_level();
        if (luminance >= sunlight)
        {
            monitor_times++;
            lum_sum+= luminance;
            lum_avr= lum_sum / monitor_times;
            if (!sun)
            {
                sun = true;
            }
            Particle.publish("Sunlight_Status","Sun\nAverage lux: "+String(lum_avr));
        }
        else if(sun)
        {
            sun = false;
            Particle.publish("Sunlight_Status", "Dark");

        }
        delay(MONITOR_DELAY);
    }
    
  
}