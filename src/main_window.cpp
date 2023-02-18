//$Id: helloworld.cc 836 2007-05-09 03:02:38Z jjongsma $ -*- c++ -*-

/* gtkmm example Copyright (C) 2002 gtkmm development team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "main_window.hpp"
#include <iostream>
#include <jack/jack.h>

typedef jack_default_audio_sample_t sample_t;

int process(jack_nframes_t nframes, void* data) {
    //printf("Process");
    MainWindow* widget = static_cast<MainWindow*>(data);
    sample_t* in_l = static_cast<sample_t*>(jack_port_get_buffer(widget->in_l, nframes));
    sample_t* in_r = static_cast<sample_t*>(jack_port_get_buffer(widget->in_r, nframes));    
    sample_t* out_l = static_cast<sample_t*>(jack_port_get_buffer(widget->out_l, nframes));
    sample_t* out_r = static_cast<sample_t*>(jack_port_get_buffer(widget->out_r, nframes));

    for (unsigned int i = 0; i < nframes; ++i) {
        out_l[i] = widget->gain * in_l[i];
        out_r[i] = widget->gain * in_r[i];
    }

    return 0;
}

MainWindow::MainWindow()
: 
m_adjustment( Gtk::Adjustment::create(0.0, 0.0, 101.0, 1.0, 1.0, 1.0) ),
m_scale(m_adjustment, Gtk::ORIENTATION_VERTICAL)
{

  set_border_width(10);

  add(m_grid);
  m_grid.set_row_homogeneous(true);
  m_grid.set_column_homogeneous(true);

  m_label.set_text("Gain");

  m_scale.set_digits(0);
  m_scale.set_value_pos(Gtk::POS_TOP);
  m_scale.set_draw_value();
  m_scale.set_inverted(); // highest value at top
  m_adjustment->signal_value_changed().connect(sigc::mem_fun(*this,
    &MainWindow::on_adjustment_value_changed));

  //grid.attach(widget, column, row, width, height); 
  m_grid.attach(m_label,0,0,1,1);
  m_grid.attach(m_scale,0,1,1,9);

  show_all_children();

  // init jack garbage
  jack_client = jack_client_open("eg-amp", JackNullOption, nullptr);

if (jack_client == nullptr)
      throw std::runtime_error("failed to initialize jack!");

    jack_set_process_callback(jack_client, &process, this);

    in_l = jack_port_register(jack_client, "inL", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    if (in_l == nullptr) {
      jack_client_close(jack_client);
      throw std::runtime_error("failed to open jack input port!");
    }

    in_r = jack_port_register(jack_client, "inR", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    if (in_r == nullptr) {
      jack_port_unregister(jack_client, in_l);
      jack_client_close(jack_client);
      throw std::runtime_error("failed to open jack input port!");
    }

    out_l = jack_port_register(jack_client, "outL", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    if (out_l == nullptr) {
      jack_client_close(jack_client);
      throw std::runtime_error("failed to open jack output port!");
    }

    out_r = jack_port_register(jack_client, "outR", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    if (out_r == nullptr) {
      jack_port_unregister(jack_client, out_l);
      jack_client_close(jack_client);
      throw std::runtime_error("failed to open jack output port!");
    }

  sample_rate = jack_get_sample_rate(jack_client);

  // if all went well, activate it!
  if (jack_activate(jack_client)) {
    jack_port_unregister(jack_client, in_l);
    jack_port_unregister(jack_client, in_r);    
    jack_port_unregister(jack_client, out_l);
    jack_port_unregister(jack_client, out_r);
    jack_client_close(jack_client);
    throw std::runtime_error("could not activate jack client!");
  }     

}

MainWindow::~MainWindow()
{
  jack_deactivate(jack_client);
  jack_port_unregister(jack_client, in_l);
  jack_port_unregister(jack_client, in_r);  
  jack_port_unregister(jack_client, out_l);
  jack_port_unregister(jack_client, out_r);
  jack_client_close(jack_client);  
}

void MainWindow::on_adjustment_value_changed()
{
  gain = m_adjustment->get_value() / 100.0f;
  printf("Gain: %f.0\n", gain);
}