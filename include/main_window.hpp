//$Id: helloworld.h 2 2003-01-21 13:41:59Z murrayc $ -*- c++ -*-

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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtkmm.h>
#include <gtkmm/scale.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <jack/jack.h>
#include <mutex>
#include <atomic>

class MainWindow : public Gtk::Window
{

public:
  jack_port_t * in_l;
  jack_port_t * in_r;
  jack_port_t * out_l;
  jack_port_t * out_r;
  float gain;  

  MainWindow();
  virtual ~MainWindow();

protected:
  void on_adjustment_value_changed();

  Gtk::Grid m_grid;

  Glib::RefPtr<Gtk::Adjustment> m_adjustment;
  Gtk::Scale m_scale;  

  Gtk::Label m_label;

private:
    jack_client_t* jack_client;
    int sample_rate;  
};

#endif // MAIN_WINDOW_H
