// license:BSD-3-Clause
// copyright-holders:Phil Stroffolino

#include "video/tigeroad_spr.h"

#include "cpu/m68000/m68000.h"
#include "cpu/m6805/m68705.h"
#include "cpu/z80/z80.h"
#include "cpu/mcs51/mcs51.h"

#include "sound/msm5205.h"
#include "sound/ymopn.h"

#include "video/bufsprite.h"

#include "emupal.h"
#include "tilemap.h"


class tigeroad_state : public driver_device
{
public:
	tigeroad_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
		, m_palette(*this, "palette")
		, m_has_coinlock(true)
		, m_spriteram(*this, "spriteram")
		, m_videoram(*this, "videoram")
		, m_audiocpu(*this, "audiocpu")
		, m_msm(*this, "msm")
		, m_gfxdecode(*this, "gfxdecode")
		, m_spritegen(*this, "spritegen")
	{ }

	void toramich(machine_config &config);
	void tigeroad(machine_config &config);
	void f1dream_comad(machine_config &config);

	void init_tigeroadb();

protected:
	required_device<cpu_device> m_maincpu;
	required_device<palette_device> m_palette;

	void soundcmd_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void videoram_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void videoctrl_w(u8 data);
	void scroll_w(offs_t offset, u16 data, u16 mem_mask = ~0);

	void main_map(address_map &map);
	// misc
	bool m_has_coinlock;

private:
	required_device<buffered_spriteram16_device> m_spriteram;
	required_shared_ptr<u16> m_videoram;
	required_device<cpu_device> m_audiocpu;
	optional_device<msm5205_device> m_msm;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<tigeroad_spr_device> m_spritegen;

	u8 m_bgcharbank;
	tilemap_t *m_bg_tilemap;
	tilemap_t *m_fg_tilemap;

	void comad_sound_io_map(address_map &map);
	void comad_sound_map(address_map &map);
	void sample_map(address_map &map);
	void sample_port_map(address_map &map);
	void sound_map(address_map &map);
	void sound_port_map(address_map &map);

	void msm5205_w(u8 data);
	TILE_GET_INFO_MEMBER(get_bg_tile_info);
	TILE_GET_INFO_MEMBER(get_fg_tile_info);
	TILEMAP_MAPPER_MEMBER(tigeroad_tilemap_scan);
	virtual void video_start() override;
	u32 screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
};


class pushman_state : public tigeroad_state
{
public:
	pushman_state(const machine_config &mconfig, device_type type, const char *tag)
		: tigeroad_state(mconfig, type, tag)
		, m_mcu(*this, "mcu")
		, m_host_semaphore(false)
		, m_mcu_semaphore(false)
		, m_host_latch(0xffff)
		, m_mcu_latch(0xffff)
		, m_mcu_output(0xffff)
		, m_mcu_latch_ctl(0xff)
	{
		m_has_coinlock = false;
	}

	void pushman(machine_config &config);
	void bballs(machine_config &config);

protected:
	virtual void machine_start() override;

private:
	u16 mcu_comm_r(offs_t offset, u16 mem_mask = ~0);
	void pushman_mcu_comm_w(offs_t offset, u16 data);
	void bballs_mcu_comm_w(u16 data);

	void mcu_pa_w(u8 data);
	void mcu_pb_w(u8 data);
	void mcu_pc_w(u8 data);

	void bballs_map(address_map &map);
	void pushman_map(address_map &map);

	required_device<m68705u_device> m_mcu;

	bool    m_host_semaphore, m_mcu_semaphore;
	u16     m_host_latch, m_mcu_latch;
	u16     m_mcu_output;
	u8      m_mcu_latch_ctl;
};

class f1dream_state : public tigeroad_state
{
public:
	f1dream_state(const machine_config &mconfig, device_type type, const char *tag)
		: tigeroad_state(mconfig, type, tag)
		, m_mcu(*this, "mcu")
		, m_ram16(*this, "ram16")
		, m_old_p3(0xff)
	{
	}

	void f1dream(machine_config &config);

protected:
	virtual void machine_start() override;

private:
	void f1dream_map(address_map &map);
	void f1dream_mcu_io(address_map &map);

	void out3_w(u8 data);

	u8 mcu_shared_r(offs_t offset);
	void mcu_shared_w(offs_t offset, u8 data);

	void to_mcu_w(u16 data);

	required_device<i8751_device> m_mcu;
	required_shared_ptr<u16> m_ram16;
	u8 m_old_p3;
};
